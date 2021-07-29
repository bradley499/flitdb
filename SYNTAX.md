# FlitDB
## Syntax
| API | Parameter | Parameter | Parameter | Parameter | API Version |
|-|-|-|-|-|-|
|[`flitdb_open`](#flitdb_open)|`filename`|[`&handler`](#handler)|`flags`||117ee|
|[`flitdb_close`](#flitdb_close)|[`&handler`](#handler)||||117ee|
|[`flitdb_version_check`](#flitdb_version_check)|||||117ee|
|[`flitdb_errmsg`](#flitdb_errmsg)|[`&handler`](#handler)||||117ee|
|[`flitdb_extract`](#flitdb_extract)|[`&handler`](#handler)|`column_position`|`row_position`||117ee|
|[`flitdb_retrieved_type`](#flitdb_retrieved_type)|[`&handler`](#handler)||||117ee|
|[`flitdb_insert`](#flitdb_insert)|[`&handler`](#handler)|`column_position`|`row_position`|`value`|117ee|
|[`flitdb_insert_int`](#flitdb_insert_int)|[`&handler`](#handler)|`column_position`|`row_position`|`value`|117ee|
|[`flitdb_insert_float`](#flitdb_insert_float)|[`&handler`](#handler)|`column_position`|`row_position`|`value`|117ee|
|[`flitdb_insert_char`](#flitdb_insert_char)|[`&handler`](#handler)|`column_position`|`row_position`|`value`|117ee|
|[`flitdb_insert_const_char`](#flitdb_insert_const_char)|[`&handler`](#handler)|`column_position`|`row_position`|`value`|117ee|
|[`flitdb_insert_bool`](#flitdb_insert_bool)|[`&handler`](#handler)|`column_position`|`row_position`|`value`|117ee|
|[`flitdb_delete`](#flitdb_delete)|[`&handler`](#handler)|`column_position`|`row_position`||117ee|
|[`flitdb_retrieve_int`](#flitdb_retrieve_int)|[`&handler`](#handler)||||117ee|
|[`flitdb_retrieve_float`](#flitdb_retrieve_float)|[`&handler`](#handler)||||117ee|
|[`flitdb_retrieve_char`](#flitdb_retrieve_char)|[`&handler`](#handler)||||117ee|
|[`flitdb_retrieve_bool`](#flitdb_retrieve_bool)|[`&handler`](#handler)||||117ee|
## APIs
### flitdb_open
Opens database through a FlitDB connection for operations. This **must** be done in order to use any subsequent FlitDB API, as the database must be accessible in order to perform any actions to it. The parameters used in conjunction with this API are as follows:

 - `filename` - This is the location of where the database is expected to be at for FlitDB to handle. Expected data type is `const char *`.

 - `handler` - See [Common Parameters](#common-parameters).

 - `flags` - FlitDB file [access flags](#access-flags) used to define certain attributes to the file.

This function returns the following *Integer* values: 
 - [FLITDB_ERROR](#operation-responses)
 - [FLITDB_NOT_FOUND](#operation-responses)
 - [FLITDB_PERM](#operation-responses)
 - [FLITDB_BUSY](#operation-responses)
 - [FLITDB_RANGE](#operation-responses)
 - [FLITDB_SUCCESS](#operation-responses)

### flitdb_close
Closes the connection to the database file, and deletes the `handler` object. No additional reads or writes can be performed with this instance once this API has been called.

The parameter defined is as follows:

 - `handler` - See [Common Parameters](#common-parameters).

This function returns the following *Integer* value:
 - [FLITDB_DONE](#operation-responses)

### flitdb_version_check
Returns the FlitDB API version used.

This function returns the following *Integer* value:
 - [FLITDB_VERSION](#value-types)

### flitdb_errmsg
Retrieves an error message from the `handler` object (if one has been provided).

The parameter defined is as follows:

 - `handler` - See [Common Parameters](#common-parameters).

This function returns a *char pointer* (char \*).

### flitdb_extract
Attempts to find a value stored in the database - through a FlitDB connection - defined by the coordinates provided in the values: `column_position`, `row_position`. Any value found, will be stored in the FlitDB `handler` object; to be retrieved by a subsequent function.

The parameters defined are as follows:

 - `column_position` - The numeric position of the column in which to access. Expected data type is `unsigned long long`, or `unsigned short` dependant on the compilation [sizing mode](#sizing-mode).

 - `row_position` - The numeric position of the row in which to access. Expected data type is `unsigned short`, or `unsigned long long` dependant on the compilation [sizing mode](#sizing-mode).

 - `handler` - See [Common Parameters](#common-parameters).

This function returns the following *Integer* values:
 - [FLITDB_ERROR](#value-types)
 - [FLITDB_RANGE](#value-types)
 - [FLITDB_CORRUPT](#value-types)
 - [FLITDB_NULL](#value-types)
 - [FLITDB_DONE](#value-types)

### flitdb_retrieved_type
Returns the data type of any value that was extracted using the [`flitdb_extract`](#flitdb_extract) function.

The parameter defined is as follows:

 - `handler` - See [Common Parameters](#common-parameters).

This function returns the following *Integer* values:

 - [FLITDB_NULL](#value-types)
 - [FLITDB_INTEGER](#value-types)
 - [FLITDB_FLOAT](#value-types)
 - [FLITDB_CHAR](#value-types)
 - [FLITDB_BOOL](#value-types)

### flitdb_insert
Attempts to write a `value` into the database - through a FlitDB connection - at the defined coordinates provided in the values: `column_position`, `row_position`.

**NOTE:** This function is only supported when compiled as a **C++** program; see alternative insertion functions - below - for **C** programs.

The parameters defined are as follows:

 - `column_position` - The numeric position of the column in which to access. Expected data type is `unsigned long long`, or `unsigned short` dependant on the compilation [sizing mode](#sizing-mode).

 - `row_position` - The numeric position of the row in which to access. Expected data type is `unsigned short`, or `unsigned long long` dependant on the compilation [sizing mode](#sizing-mode).

 - `handler` - See [Common Parameters](#common-parameters).

 - `value` - The value that is to be stored within the database. Valid datatypes are:

   - `int`
   - `float`
   - `char* `
   - `const char*`
   - `bool`

This function returns the following *Integer* values:

 - [FLITDB_ERROR](#operation-responses)
 - [FLITDB_READONLY](#operation-responses)
 - [FLITDB_RANGE](#operation-responses)
 - [FLITDB_CORRUPT](#operation-responses)
 - [FLITDB_DONE](#operation-responses)

### flitdb_insert_int
Attempts to write an integer `value` into the database - through a FlitDB connection - at the defined coordinates provided in the values: `column_position`, `row_position`.

The parameters defined are as follows:

 - `column_position` - The numeric position of the column in which to access. Expected data type is `unsigned long long`, or `unsigned short` dependant on the compilation [sizing mode](#sizing-mode).

 - `row_position` - The numeric position of the row in which to access. Expected data type is `unsigned short`, or `unsigned long long` dependant on the compilation [sizing mode](#sizing-mode).

 - `handler` - See [Common Parameters](#common-parameters).

 - `value` - The integer value that is to be stored within the database. Expected data type is `int`.

This function returns the following *Integer* values:

 - [FLITDB_ERROR](#operation-responses)
 - [FLITDB_READONLY](#operation-responses)
 - [FLITDB_RANGE](#operation-responses)
 - [FLITDB_CORRUPT](#operation-responses)
 - [FLITDB_DONE](#operation-responses)

### flitdb_insert_float
Attempts to write a float `value` into the database - through a FlitDB connection - at the defined coordinates provided in the values: `column_position`, `row_position`.

The parameters defined are as follows:

 - `column_position` - The numeric position of the column in which to access. Expected data type is `unsigned long long`, or `unsigned short` dependant on the compilation [sizing mode](#sizing-mode).

 - `row_position` - The numeric position of the row in which to access. Expected data type is `unsigned short`, or `unsigned long long` dependant on the compilation [sizing mode](#sizing-mode).
 
 - `handler` - See [Common Parameters](#common-parameters).

 - `value` - The float value that is to be stored within the database. Expected data type is `float`.

This function returns the following *Integer* values:

 - [FLITDB_ERROR](#operation-responses)
 - [FLITDB_READONLY](#operation-responses)
 - [FLITDB_RANGE](#operation-responses)
 - [FLITDB_CORRUPT](#operation-responses)
 - [FLITDB_DONE](#operation-responses)

### flitdb_insert_char
Attempts to write a *char pointer* (char \*) `value` into the database - through a FlitDB connection - at the defined coordinates provided in the values: `column_position`, `row_position`.

The parameters defined are as follows:

 - `column_position` - The numeric position of the column in which to access. Expected data type is `unsigned long long`, or `unsigned short` dependant on the compilation [sizing mode](#sizing-mode).

 - `row_position` - The numeric position of the row in which to access. Expected data type is `unsigned short`, or `unsigned long long` dependant on the compilation [sizing mode](#sizing-mode).

 - `handler` - See [Common Parameters](#common-parameters).

 - `value` - The *char pointer* (const char \*) value that is to be stored within the database. Expected data type is `char *`.

This function returns the following *Integer* values:

 - [FLITDB_ERROR](#operation-responses)
 - [FLITDB_READONLY](#operation-responses)
 - [FLITDB_RANGE](#operation-responses)
 - [FLITDB_CORRUPT](#operation-responses)
 - [FLITDB_DONE](#operation-responses)

### flitdb_insert_const_char
Attempts to write a *const char pointer* (char \*) `value` into the database - through a FlitDB connection - at the defined coordinates provided in the values: `column_position`, `row_position`.

The parameters defined are as follows:

 - `column_position` - The numeric position of the column in which to access. Expected data type is `unsigned long long`, or `unsigned short` dependant on the compilation [sizing mode](#sizing-mode).

 - `row_position` - The numeric position of the row in which to access. Expected data type is `unsigned short`, or `unsigned long long` dependant on the compilation [sizing mode](#sizing-mode).

 - `handler` - See [Common Parameters](#common-parameters).

 - `value` - The *const char pointer* (const char \*) value that is to be stored within the database. Expected data type is `const char *`.

This function returns the following *Integer* values:

 - [FLITDB_ERROR](#operation-responses)
 - [FLITDB_READONLY](#operation-responses)
 - [FLITDB_RANGE](#operation-responses)
 - [FLITDB_CORRUPT](#operation-responses)
 - [FLITDB_DONE](#operation-responses)

### flitdb_insert_bool
Attempts to write a boolean `value` into the database - through a FlitDB connection - at the defined coordinates provided in the values: `column_position`, `row_position`.

The parameters defined are as follows:

 - `column_position` - The numeric position of the column in which to access. Expected data type is `unsigned long long`, or `unsigned short` dependant on the compilation [sizing mode](#sizing-mode).

 - `row_position` - The numeric position of the row in which to access. Expected data type is `unsigned short`, or `unsigned long long` dependant on the compilation [sizing mode](#sizing-mode).

 - `handler` - See [Common Parameters](#common-parameters).

 - `value` - The boolean value that is to be stored within the database. Expected data type is `bool`.

This function returns the following *Integer* values:

 - [FLITDB_ERROR](#operation-responses)
 - [FLITDB_READONLY](#operation-responses)
 - [FLITDB_RANGE](#operation-responses)
 - [FLITDB_CORRUPT](#operation-responses)
 - [FLITDB_DONE](#operation-responses)

### flitdb_delete
Attempts to delete a value (if present) at the provided coordinates within the database; provided in the values: `column_position`, `row_position`.

The parameters defined are as follows:

 - `column_position` - The numeric position of the column in which to access. Expected data type is `unsigned long long`, or `unsigned short` dependant on the compilation [sizing mode](#sizing-mode).

 - `row_position` - The numeric position of the row in which to access. Expected data type is `unsigned short`, or `unsigned long long` dependant on the compilation [sizing mode](#sizing-mode).

 - `handler` - See [Common Parameters](#common-parameters).

This function returns the following *Integer* values:

 - [FLITDB_ERROR](#operation-responses)
 - [FLITDB_READONLY](#operation-responses)
 - [FLITDB_RANGE](#operation-responses)
 - [FLITDB_CORRUPT](#operation-responses)
 - [FLITDB_DONE](#operation-responses)

### flitdb_retrieve_int
Returns the integer value that was extracted using the [`flitdb_extract`](#flitdb_extract) function. If the datatype of the extracted value is not of [`FLITDB_INTEGER`](#FLITDB_INTEGER), the value will default to `0`.

The parameter defined is as follows:

 - `handler` - See [Common Parameters](#common-parameters).

This function returns a *Integer* value.

### flitdb_retrieve_float
Returns the float value that was extracted using the [`flitdb_extract`](#flitdb_extract) function. If the datatype of the extracted value is not of [`FLITDB_FLOAT`](#FLITDB_FLOAT), the value will default to `0`.

The parameter defined is as follows:

 - `handler` - See [Common Parameters](#common-parameters).

This function returns a *Float* value.

### flitdb_retrieve_char
Returns the char value that was extracted using the [`flitdb_extract`](#flitdb_extract) function. If the datatype of the extracted value is not of [`FLITDB_CHAR`](#FLITDB_CHAR), the value will default to a null terminated char array; consisting of no value.

The parameter defined is as follows:

 - `handler` - See [Common Parameters](#common-parameters).

This function returns a *char pointer* (char \*).

### flitdb_retrieve_bool
Returns the boolean value that was extracted using the [`flitdb_extract`](#flitdb_extract) function. If the datatype of the extracted value is not of [`FLITDB_BOOL`](#FLITDB_BOOL), the value will default to `false`.

The parameter defined is as follows:

 - `handler` - See [Common Parameters](#common-parameters).

This function returns a *Boolean* value.

## Common Parameters
#### `handler`
A handler is a FlitDB object, which is used to perform operations on the database. A FlitDB object is very easy to create, all you need to do is the following:

	flitdb *flit;

With `flit` being any name that you wish, as it is a variable name. You do not need to declare the FlitDB object as being initialized with any value, as that is handled all by the function [`flitdb_open`](#flitdb_open), allowing for a easy creation and deletion - when used with [`flitdb_close`](#flitdb_close).

**NOTE:** The FlitDB object is expected to be passed by reference to all functions (example: `&flit`).

## Compilation Options
Within the file [`flit.h`](flitdb/flit.h), you can limit the total operable size of the database, by setting the compilation value of `FLITDB_SIZING_MODE` to one of three different values; this definition of a sizing mode is put in place to further limit the amount of data that FlitDB will have access to. Additionally, you also have the option to allow memory mapping the database file, this results in a faster overall "read" time as the reading of the database can be accomplished from reading the mapped memory database file, instead of reading from *disk*; this feature is currently only supported when the `FLITDB_SIZING_MODE` is set to `FLITDB_SIZING_MODE_TINY` as well as opening the database with the [`FLITDB_READONLY`](#access-flags) option set, and requires enabling, which can be done by setting the value of `FLITDB_MMAP_ALLOWED` to `1` (default value - is enabled by default).
### Sizing mode
|Mode|Limitations|Maximum size|
|-|-|-|
|FLITDB_SIZING_MODE_BIG|Default - allows for writing up to 65536 columns (and even more with [`FLITDB_UNSAFE`](#access-flags)), and up to 65536 rows.|`281.47` TB|
|FLITDB_SIZING_MODE_SMALL|Allows up to 256 columns and rows. Cannot be used in conjunction with [`FLITDB_UNSAFE`](#access-flags).|`4.26` GB|
|FLITDB_SIZING_MODE_TINY|Allows up to 16 columns and rows. Cannot be used in conjunction with [`FLITDB_UNSAFE`](#access-flags).|`14.74` MB|

## FlitDB constants
### Access Flags
|Constant|Meaning|
|-|-|
|`FLITDB_CREATE`|Create a database if not existent.|
|`FLITDB_READONLY`|Only allow the reading of the database.|
|`FLITDB_UNSAFE`|Discard all safety protocols to allow for larger database - exceeding [maximum size](#sizing-mode).|
### Operation responses
|Constant|Meaning|
|-|-|
|`FLITDB_SUCCESS`|Successful operation.|
|`FLITDB_ERROR`|Unsuccessful operation.|
|`FLITDB_PERM`|Permission denied.|
|`FLITDB_BUSY`|The database file is locked.|
|`FLITDB_NOT_FOUND`|The database file is not found.|
|`FLITDB_CORRUPT`|The database file is malformed.|
|`FLITDB_RANGE`|The requested range is outside the range of the database.|
|`FLITDB_DONE`|The operation was completed successfully.|
|`FLITDB_NULL`|The operation resulted in a null lookup.|
|`FLITDB_READONLY`|A write to the database was attempted, when an [access flag](#access-flags) for `FLITDB_READONLY` was set.|
### Value types
|Constant|Meaning|
|-|-|
|`FLITDB_NULL`|The value requested is of no type - does not exist.|
|`FLITDB_INTEGER`|The value type of integer.|
|`FLITDB_FLOAT`|The value type of float.|
|`FLITDB_CHAR`|The value type of char.|
|`FLITDB_BOOL`|The value type of bool.|
|`FLITDB_VERSION`|The current FlitDB API version.|
