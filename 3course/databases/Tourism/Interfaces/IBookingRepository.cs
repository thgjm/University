using System;
using System.Collections.Generic;
using Tourism.Models;

namespace Tourism.Interfaces
{
    public interface IBookingRepository
    {
        Guid Create(string email, Guid scheduleId, Guid? roomTypeId);
        
        decimal GetPriceForBooking(Guid bookingId);
        
        void Confirm(Guid bookingId);
        IEnumerable<BookingViewModel> GetAllBookings();
    }
}