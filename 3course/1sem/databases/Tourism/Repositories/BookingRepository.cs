using System;
using System.Collections.Generic;
using System.Data.SqlClient;
using System.Data;
using Tourism.Interfaces;
using Tourism.Models;

namespace Tourism.Repositories
{
    public class BookingRepository : IBookingRepository
    {
        private readonly SqlTransaction _transaction;
        private readonly SqlConnection _connection;
        public BookingRepository(SqlTransaction transaction)
        {
            _transaction = transaction;
            _connection = transaction.Connection;
        }
         public Guid Create(string email, Guid scheduleId, Guid? roomTypeId)
        {
            using (var cmd = new SqlCommand("usp_CreateNewBooking", _connection, _transaction))
            {
                cmd.CommandType = CommandType.StoredProcedure;
                cmd.Parameters.AddWithValue("@TravelerEmail", email);
                cmd.Parameters.AddWithValue("@ScheduleID", scheduleId);
                cmd.Parameters.AddWithValue("@RoomTypeID", (object)roomTypeId ?? DBNull.Value);
                var outputParam = cmd.Parameters.Add("@NewBookingID", SqlDbType.UniqueIdentifier);
                outputParam.Direction = ParameterDirection.Output;
                cmd.ExecuteNonQuery();
                return (Guid)outputParam.Value;
            }
        }
        
        public decimal GetPriceForBooking(Guid bookingId)
        {
            string sql = "SELECT total_price FROM Booking WHERE id = @BookingID";
            using (var cmd = new SqlCommand(sql, _connection, _transaction))
            {
                cmd.CommandType = CommandType.Text;
                cmd.Parameters.AddWithValue("@BookingID", bookingId);
                var result = cmd.ExecuteScalar();
                if (result != null && result != DBNull.Value)
                    return Convert.ToDecimal(result);
                throw new InvalidOperationException($"Не вдалося отримати ціну для бронювання ID: {bookingId}.");
            }
        }

        public void Confirm(Guid bookingId)
        {
            using (var cmd = new SqlCommand("usp_ConfirmBooking", _connection, _transaction))
            {
                cmd.CommandType = CommandType.StoredProcedure;
                cmd.Parameters.AddWithValue("@BookingID", bookingId);
                cmd.ExecuteNonQuery();
            }
        }

        public IEnumerable<BookingViewModel> GetAllBookings()
        {
            var list = new List<BookingViewModel>();
            string sql = "SELECT BookingID, TravelerName, TourTitle, total_price FROM v_AllBookings";
            using (var cmd = new SqlCommand(sql, _connection, _transaction))
            {
                cmd.CommandType = CommandType.Text;
                using (var reader = cmd.ExecuteReader())
                    while (reader.Read())
                        list.Add(new BookingViewModel
                        {
                            BookingId = reader.GetGuid(0),
                            TravelerName = reader.GetString(1),
                            TourTitle = reader.GetString(2),
                            TotalPrice = reader.GetDecimal(3)
                        });
            }
            return list;
        }
    }
}