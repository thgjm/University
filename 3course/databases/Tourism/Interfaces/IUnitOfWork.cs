using System;
using Tourism.Interfaces;


public interface IUnitOfWork : IDisposable
    {
        IBookingRepository Bookings { get; }
        void Commit();
        void Rollback();
    }