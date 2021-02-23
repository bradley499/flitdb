# FlitDB
## Syntax
| API | Parameter | Parameter | Parameter | Parameter | API Version |
|-|-|-|-|-|-|
|[`flitdb_setup`](#flitdb_setup)|`filename`|[`&handler`](#handler)|`flags`||52d9|
|[`flitdb_close`](#flitdb_close)|[`&handler`](#handler)||||52d9|
|[`flitdb_errmsg`](#flitdb_errmsg)|[`&handler`](#handler)||||52d9|
|[`flitdb_extract`](#flitdb_extract)|[`&handler`](#handler)|`column_position`|`row_position`||52d9|
|[`flitdb_retrieved_type`](#flitdb_retrieved_type)|[`&handler`](#handler)||||52d9|
|[`flitdb_insert`](#flitdb_insert)|[`&handler`](#handler)|`column_position`|`row_position`|`value`|52d9|
|[`flitdb_insert_int`](#flitdb_insert_int)|[`&handler`](#handler)|`column_position`|`row_position`|`value`|52d9|
|[`flitdb_insert_double`](#flitdb_insert_double)|[`&handler`](#handler)|`column_position`|`row_position`|`value`|52d9|
|[`flitdb_insert_float`](#flitdb_insert_float)|[`&handler`](#handler)|`column_position`|`row_position`|`value`|52d9|
|[`flitdb_insert_char`](#flitdb_insert_char)|[`&handler`](#handler)|`column_position`|`row_position`|`value`|52d9|
|[`flitdb_insert_const_char`](#flitdb_insert_const_char)|[`&handler`](#handler)|`column_position`|`row_position`|`value`|52d9|
|[`flitdb_insert_bool`](#flitdb_insert_bool)|[`&handler`](#handler)|`column_position`|`row_position`|`value`|52d9|
|[`flitdb_delete`](#flitdb_delete)|[`&handler`](#handler)|`column_position`|`row_position`||52d9|
|[`flitdb_retrieve_int`](#flitdb_retrieve_int)|[`&handler`](#handler)||||52d9|
|[`flitdb_retrieve_double`](#flitdb_retrieve_double)|[`&handler`](#handler)||||52d9|
|[`flitdb_retrieve_float`](#flitdb_retrieve_float)|[`&handler`](#handler)||||52d9|
|[`flitdb_retrieve_char`](#flitdb_retrieve_char)|[`&handler`](#handler)||||52d9|
|[`flitdb_retrieve_bool`](#flitdb_retrieve_bool)|[`&handler`](#handler)||||52d9|
## APIs
### flitdb_setup
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

### flitdb_errmsg
Retrieves an error message from the `handler` object (if one has been provided).

The parameter defined is as follows:

 - `handler` - See [Common Parameters](#common-parameters).

This function returns a *char pointer* (char \*).

### flitdb_extract
Attempts to find a value stored in the database - through a FlitDB connection - defined by the coordinates provided in the values: `column_position`, `row_position`. Any value found, will be stored in the FlitDB `handler` object; to be retrieved by a subsequent function.

The parameters defined are as follows:

 - `column_position` - The numeric position of the column in which to access. Expected data type is `unsigned long int`.

 - `row_position` - The numeric position of the row in which to access. Expected data type is `unsigned long int`.

 - `handler` - See [Common Parameters](#common-parameters).

This function returns the following *Integer* values:
 - [FLITDB_ERROR](#value-types)
 - [FLITDB_RANGE](#value-types)
 - [FLITDB_CORRUPT](#value-types)
 - [FLITDB_NULL](#value-types)

### flitdb_retrieved_type
Returns the data type of any value that was extracted using the [`flitdb_extract`](#flitdb_extract) function.

The parameter defined is as follows:

 - `handler` - See [Common Parameters](#common-parameters).

This function returns the following *Integer* values:

 - [FLITDB_NULL](#value-types)
 - [FLITDB_INTEGER](#value-types)
 - [FLITDB_DOUBLE](#value-types)
 - [FLITDB_FLOAT](#value-types)
 - [FLITDB_CHAR](#value-types)
 - [FLITDB_BOOL](#value-types)

### flitdb_insert
Attempts to write a `value` into the database - through a FlitDB connection - at the defined coordinates provided in the values: `column_position`, `row_position`.

**NOTE:** This function is only supported when compiled as a **C++** program; see alternative insertion functions - below - for **C** programs.

The parameters defined are as follows:

 - `column_position` - The numeric position of the column in which to access. Expected data type is `unsigned long int`.

 - `row_position` - The numeric position of the row in which to access. Expected data type is `unsigned long int`.

 - `handler` - See [Common Parameters](#common-parameters).

 - `value` - The value that is to be stored within the database. Valid datatypes are:

   - `signed long long int`
   - `long double`
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

 - `column_position` - The numeric position of the column in which to access. Expected data type is `unsigned long int`.

 - `row_position` - The numeric position of the row in which to access. Expected data type is `unsigned long int`.

 - `handler` - See [Common Parameters](#common-parameters).

 - `value` - The integer value that is to be stored within the database. Expected data type is `signed long long int`.

This function returns the following *Integer* values:

 - [FLITDB_ERROR](#operation-responses)
 - [FLITDB_READONLY](#operation-responses)
 - [FLITDB_RANGE](#operation-responses)
 - [FLITDB_CORRUPT](#operation-responses)
 - [FLITDB_DONE](#operation-responses)

### flitdb_insert_double
Attempts to write a double `value` into the database - through a FlitDB connection - at the defined coordinates provided in the values: `column_position`, `row_position`.

The parameters defined are as follows:

 - `column_position` - The numeric position of the column in which to access. Expected data type is `unsigned long int`.

 - `row_position` - The numeric position of the row in which to access. Expected data type is `unsigned long int`.

 - `handler` - See [Common Parameters](#common-parameters).

 - `value` - The double value that is to be stored within the database. Expected data type is `long double long long int`.

This function returns the following *Integer* values:

 - [FLITDB_ERROR](#operation-responses)
 - [FLITDB_READONLY](#operation-responses)
 - [FLITDB_RANGE](#operation-responses)
 - [FLITDB_CORRUPT](#operation-responses)
 - [FLITDB_DONE](#operation-responses)

### flitdb_insert_float
Attempts to write a float `value` into the database - through a FlitDB connection - at the defined coordinates provided in the values: `column_position`, `row_position`.

The parameters defined are as follows:

 - `column_position` - The numeric position of the column in which to access. Expected data type is `unsigned long int`.

 - `row_position` - The numeric position of the row in which to access. Expected data type is `unsigned long int`.

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

 - `column_position` - The numeric position of the column in which to access. Expected data type is `unsigned long int`.

 - `row_position` - The numeric position of the row in which to access. Expected data type is `unsigned long int`.

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

 - `column_position` - The numeric position of the column in which to access. Expected data type is `unsigned long int`.

 - `row_position` - The numeric position of the row in which to access. Expected data type is `unsigned long int`.

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

 - `column_position` - The numeric position of the column in which to access. Expected data type is `unsigned long int`.

 - `row_position` - The numeric position of the row in which to access. Expected data type is `unsigned long int`.

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

 - `column_position` - The numeric position of the column in which to access. Expected data type is `unsigned long int`.

 - `row_position` - The numeric position of the row in which to access. Expected data type is `unsigned long int`.

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

### flitdb_retrieve_double
Returns the double value that was extracted using the [`flitdb_extract`](#flitdb_extract) function. If the datatype of the extracted value is not of [`FLITDB_DOUBLE`](#FLITDB_DOUBLE), the value will default to `0`.

The parameter defined is as follows:

 - `handler` - See [Common Parameters](#common-parameters).

This function returns a *Double* value.

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

### Common Parameters
#### `handler`
A handler is a FlitDB object, which is used to perform operations on the database. A FlitDB object is very easy to create, all you need to do is the following:

	flitdb *flit;

With `flit` being any name that you wish, as it is a variable name. You do not need to declare the FlitDB object as being initialized with any value, as that is handled all by the function [`flitdb_setup`](#flitdb_setup), allowing for a easy creation and deletion - when used with [`flitdb_close`](#flitdb_close).

**NOTE:** The FlitDB object is expected to be passed by reference to all functions (example: `&flit`).

## FlitDB constants
### Access Flags
|Constant|Meaning|
|-|-|
|`FLITDB_CREATE`|Create a database if not existent.|
|`FLITDB_READONLY`|Only allow the reading of the database.|
|`FLITDB_UNSAFE`|Discard all safety protocols to allow for larger database.|
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
|`FLITDB_NULL`|The value requested is of no type - undefined.|
|`FLITDB_INTEGER`|The value type of integer.|
|`FLITDB_DOUBLE`|The value type of double.|
|`FLITDB_FLOAT`|The value type of float.|
|`FLITDB_CHAR`|The value type of char.|
|`FLITDB_BOOL`|The value type of bool.|
