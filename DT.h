#ifndef DT_H
#define DT_H

namespace DT
{
  const int TYPE_MASK = 0xFF;
  const int UNIQUE_ID = 0x00800000;
  const int NOT_FOUND = -1;

  namespace ColumnTypes
  {
    enum
    {
      INT     = 0,
      STRING  = 1,
      INT64   = 3,
      BIN     = 4,
      DOUBLE  = 6,
      UNKNOWN	= -1
    };
  };

  namespace ColumnFlags
  {
    enum
    {
      NONE		 = 0,
      DONT_SAVE = 0x100,
      KEY		   = 0x200,
      SECRET   = 0x400,

      IS_LOADED  = 0x100000,
      IS_DEFINED = 0x200000,

      CRYPT	= 0xFF000,
      XOR		= 0x01000
    };
  };

  namespace RowFlags
  {
    enum
    {
      NONE = 0,
      ROW_ID
    };
  };

  namespace DataFlags
  {
    enum
    {
      NONE = 0,

      LAST_ID = 1,
      PASSWORD = 2,
      DB_ID = 4,

      PARAMS = 8,
      CREATED = 0x10,
      MODIFIED = 0x20,
      LAST_BACKUP = 0x40,
      PASS_SALT = 0x80,
      XOR_SALT = 0x100,
    };
  };

  namespace FileFlags
  {
    enum
    {
      NONE = 0,
      FRAGMENTED = 0x10000,
      CRYPT_ALL = 0x20000,
    };
  };

  namespace Errors
  {
    enum
    {
      SUCCESS = 0,

      NO_COLUMN = 100,
      NO_ROW = 200,
      BAD_TYPE = 300,

      FILE_NOT_FOUND = 0x1000,
      NOT_INITIALIZED = 0x1001,
      NOT_AUTHENTICATED = 0x1002,
      NOT_OPENED = 0x1003,
      BAD_FORMAT = 0x1004,
      BAD_VERSION = 0x1005,
      ROW_NOT_FOUND = 0x1006,
      BAD_PARAMETER = 0x1007,
      WRITE_ERROR = 0x1008,
      ALREADY_LOADED = 0x1009,
      NOT_LOADED = 0x1010,
      NOT_CHANGED = 0x1011,
      READ_ERROR = 0x1012,
      FILE_ERROR = 0x10000000,

      SKIPPED = 1,
      NOTHING_TO_READ = 2,
    };
  };

  namespace XorKeys
  {
    enum
    {
      NONE = 0,
      CONFIG,
      HISTORY
    };
  };
};

#endif
