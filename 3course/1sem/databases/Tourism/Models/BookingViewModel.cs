using MongoDB.Bson;
using MongoDB.Bson.Serialization.Attributes;
using System;

namespace Tourism.Models
{
        public class BookingViewModel
    {
        public Guid BookingId { get; set; }
        public string TravelerName { get; set; }
        public string TourTitle { get; set; }
        public decimal TotalPrice { get; set; }
    }
}