using System;
using System.Data;
using Microsoft.Data.SqlClient;
using System.Text.Json; 
using System.Collections.Generic;
using System.Text.Json.Nodes;

namespace Tourism.Services
{
    public class SqlLogRepository
    {
        private readonly string _connectionString;

        public SqlLogRepository(string connectionString)
        {
            _connectionString = connectionString ?? throw new ArgumentNullException(nameof(connectionString));
        }

        public void Log(string action, string userEmail, object details)
        {
            string jsonString = JsonSerializer.Serialize(details); 
            const string sql = @"
                INSERT INTO SqlAuditLog (ActionName, UserEmail, DetailsJson) 
                VALUES (@Action, @UserEmail, @JsonDetails)";

            try
            {
                using (var conn = new SqlConnection(_connectionString))
                {
                    conn.Open();
                    using (var cmd = new SqlCommand(sql, conn))
                    {
                        cmd.CommandType = CommandType.Text;
                        cmd.Parameters.AddWithValue("@Action", action);
                        cmd.Parameters.AddWithValue("@UserEmail", userEmail);
                        cmd.Parameters.AddWithValue("@JsonDetails", jsonString);
                        cmd.ExecuteNonQuery();
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"[SqlLogRepository Error - JSON]: Не вдалося записати лог у SQL. {ex.Message}");
            }
        }

        public void LogNormalized(string action, string userEmail, object details)
        {
            using (var conn = new SqlConnection(_connectionString))
            {
                conn.Open();
                SqlTransaction transaction = conn.BeginTransaction(); 

                try
                {
                    const string headerSql = @"
                        DECLARE @NewHeaderId UNIQUEIDENTIFIER;
                        SET @NewHeaderId = NEWID(); -- Генеруємо GUID
                        INSERT INTO LogHeader (HeaderId, ActionName, UserEmail) 
                        VALUES (@NewHeaderId, @Action, @UserEmail);
                        SELECT @NewHeaderId;";
                    
                    Guid headerId = Guid.Empty;
                    using (var cmd = new SqlCommand(headerSql, conn, transaction))
                    {
                        cmd.Parameters.AddWithValue("@Action", action);
                        cmd.Parameters.AddWithValue("@UserEmail", userEmail);
                        
                        object result = cmd.ExecuteScalar();
                        if (result != null && result != DBNull.Value)
                            headerId = (Guid)result; 
                        else
                            throw new Exception("Не вдалося отримати HeaderId після вставки.");
                    }

                    var jsonNode = JsonSerializer.Deserialize<Dictionary<string, JsonElement>>(JsonSerializer.Serialize(details));
                    const string detailSql = @"
                        INSERT INTO LogDetails (HeaderID, DetailKey, DetailValue) 
                        VALUES (@HeaderID, @KeyName, @ValueString)";
                    
                    using (var cmd = new SqlCommand(detailSql, conn, transaction))
                    {
                        cmd.Parameters.AddWithValue("@HeaderID", headerId);
                        var keyParam = cmd.Parameters.Add("@KeyName", SqlDbType.NVarChar);
                        var valueParam = cmd.Parameters.Add("@ValueString", SqlDbType.NVarChar, -1);

                        foreach (var kvp in jsonNode)
                        {
                            keyParam.Value = kvp.Key;
                            valueParam.Value = kvp.Value.ToString();
                            cmd.ExecuteNonQuery();
                        }
                    }

                    transaction.Commit(); 
                }
                catch (Exception ex)
                {
                    transaction.Rollback(); 
                    Console.WriteLine($"[SqlLogRepository Error - Normalized]: Не вдалося записати нормалізований лог. {ex.Message}");
                }
            }
        }
    }
}