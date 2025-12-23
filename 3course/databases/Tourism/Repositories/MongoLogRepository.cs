using System;
using System.Threading.Tasks;
using MongoDB.Driver;
using MongoDB.Bson;
using Tourism.Models;
using System.Text.Json;
using MongoDB.Bson.Serialization; 
using MongoDB.Bson.Serialization.Serializers;
using MongoDB.Bson.Serialization.Options; 

namespace Tourism.Repositories
{
    public class MongoLogRepository
    {
        private readonly IMongoCollection<AuditLog> _logs;

        public MongoLogRepository(string connectionString, string databaseName)
        {
            BsonSerializer.RegisterSerializer(new GuidSerializer(GuidRepresentation.Standard));

            var client = new MongoClient(connectionString);
            var database = client.GetDatabase(databaseName);
            _logs = database.GetCollection<AuditLog>("SystemLogs");
        }

        public async Task LogAsync(string action, string email, object details)
        {
            string jsonString = JsonSerializer.Serialize(details);
            var detailsBson = BsonDocument.Parse(jsonString);

            var logEntry = new AuditLog
            {
                Action = action,
                UserEmail = email,
                Timestamp = DateTime.UtcNow,
                Details = detailsBson
            };
            
            await _logs.InsertOneAsync(logEntry);
        }
    }
}