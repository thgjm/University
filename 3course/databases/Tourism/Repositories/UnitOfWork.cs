using System;
using System.Collections.Generic;
using System.Data.SqlClient;
using Tourism.Interfaces;
using Tourism.Models;

namespace Tourism.Repositories
{
    public class UnitOfWork : IUnitOfWork
    {
        private readonly SqlConnection _connection;
        private SqlTransaction _transaction;
        private IBookingRepository _bookingRepository;
        private bool _disposed;
        public UnitOfWork(string connectionString)
        {
            _connection = new SqlConnection(connectionString);
            _connection.Open();
            _transaction = _connection.BeginTransaction();
        }

        public IBookingRepository Bookings
        {
            get { return _bookingRepository ??= new BookingRepository(_transaction); }
        }

        public void Commit()
        {
            try { _transaction.Commit(); }
            catch { _transaction.Rollback(); throw; }
        }

        public void Rollback() { _transaction.Rollback(); }

        public void Dispose()
        {
            if (!_disposed)
            {
                _transaction?.Dispose();
                _connection?.Dispose();
                _disposed = true;
            }
        }
    }
}