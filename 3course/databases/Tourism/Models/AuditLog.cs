using System;
using MongoDB.Bson;
using MongoDB.Bson.Serialization.Attributes;

namespace Tourism.Models
{
    public class AuditLog
    {
        [BsonId]
        public ObjectId Id { get; set; }

        public string Action { get; set; } // наприклад Booking_Created, Error_Occurred
        
        public string UserEmail { get; set; }

        [BsonDateTimeOptions(Kind = DateTimeKind.Utc)]
        public DateTime Timestamp { get; set; }

        // поле для зберігання деталей події
        public BsonDocument Details { get; set; } 
    }
}