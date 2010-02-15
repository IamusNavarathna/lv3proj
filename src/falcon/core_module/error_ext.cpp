/*
   FALCON - The Falcon Programming Language.
   FILE: error_ext.cpp

   Falcon error class and core subclasses
   -------------------------------------------------------------------
   Author: Giancarlo Niccolai
   Begin: Thu, 14 Aug 2008 00:17:31 +0200

   -------------------------------------------------------------------
   (C) Copyright 2008: the FALCON developers (see list in AUTHORS file)

   See LICENSE file for licensing details.
*/

#include "core_module.h"
#include <falcon/sys.h>

namespace Falcon {
namespace core {

/*#
   @group errors The Falcon Error System.
   @brief Falcon classes reflecting internal errors.

   This is the list of classes used by falcon core module to report the scripts (or
   embedding applications) about runtime errors.
*/

/*#
   @class Error
   @brief Internal VM and runtime error reflection class.
   @ingroup errors
   @ingroup general_purpose

   @optparam code A numeric error code.
   @optparam description A textual description of the error code.
   @optparam extra A descriptive message explaining the error conditions.

   In case the error code is a well known code (i.e. one of the codes
   known by the engine), the description of the error will be automatically provided.

   To provide an error message without setting the code description, use directly the
   @a Error.message property after having created the object.

   The Error class is used by the virtual machine and by the Falcon Feathers
   functions to communicate to the scripts, and eventually to the embedding
   application, about error conditions. It is also available to extension modules,
   and to the script themselves, that can create error instances that can be
   cached internally or returned to the embedder.

   A complete error code is formed by two letters indicating the error origin,
   and a numeric code specifying the correct error name. By convention, one and
   only one error description may be associated with one error code. The error
   @i message is free to be used to carry more specific informations about the error
   conditions.

   Use the comment parameter when the error message is generic, and/or the error
   may be reported because of various reasons, or to give an hint about how to
   avoid the error.

   Error codes below 5000 are reserved to Falcon engine and officially recognized
   modules. Extension modules should issue errors above 5001, unless raising well
   known error codes that are encoded and described directly by the Falcon Engine
   (i.e. a very common error code is 901 - invalid parameters when a user makes a
   mistake in calling a script function).

   All the elements in the error class are automatically initialized by the
   constructor, except for the code, the message and the description. As some error
   are created by binary modules, which are not executed by the VM, the
   informations about the line and the program counter that generated the error may
   not always be available.

   The toString() method returns a string representation of the error, which
   includes all the available informations (except for system error description).
   In this version, access to the TraceBack class has been removed from scripts.

   @prop description Textual description for the error code of this error.

   @prop message Arbitrary text used to better explain and define the error
                 conditions. Consider this as a "free text".

   @prop systemError If the error was caused by a failure during an OS operation,
                     this this property contains the error code indicating the cause of the failure.
                     Otherwise, it will be 0.


   @prop module Name of the module where the error has been generated.
   @prop symbol Symbol name (function or method) where the error has been raised.
   @prop line Line at which the error happened. If not applicable
             (i.e. because the error is not generated by a Falcon script) is 0.

   @prop pc Program counter of the instruction that raised the error.
            If not applicable (i.e. if the VM wasn't running when the error has been raised)
            the number will be 0.

   @prop subErrors Array of sub-errors.
                  Some error generating facilities may delay error
                  reporting to complete some operations, and then report all the
                  errors at once, encapsulated in a top-level failure signaling error.
                  It's the case of the reflexive compiler, which, in case of
                  errors during compilation of source code, would record all the
                  errors and store them in a generic "syntax error" exception.
                  This property stores a vector of the single sub-errors that have
                  caused operation failure.
*/

// Separate "code" property to test for @property command

FALCON_FUNC  Error_init ( ::Falcon::VMachine *vm )
{
   core::ErrorObject *einst = static_cast<core::ErrorObject* >(vm->self().asObject());

   // subclasses may have already given a value to the userdata.
   Falcon::Error *err = einst->getError();
   if( err == 0 )
   {
      err = new Falcon::Error( ErrorParam( 0, __LINE__ ).
         module( "core" ) );
      einst->setUserData( err );
   }

   // declare that the script has created it
   err->origin( e_orig_script );
   vm->fillErrorContext( err );

   // filling properties
   Item *param = vm->param( 0 );
   if ( param != 0 && param->type() != FLC_ITEM_NIL  )
      err->errorCode( (int) param->forceInteger() );

   param = vm->param( 1 );
   if ( param != 0 && param->isString() )
      err->errorDescription( *param->asString() );

   param = vm->param( 2 );
   if ( param != 0 && param->isString() )
      err->extraDescription( *param->asString() );
}

/*#
   @method toString Error
   @brief Creates a textual representation of the error.

   This method uses the standard Falcon error representation to render
   the error codes, descriptions and stack traces into a string. Suberrors
   are also considered.

   To get only a descriptive string of the error without its stack trace,
   use the @a Error.heading method.
*/
FALCON_FUNC  Error_toString ( ::Falcon::VMachine *vm )
{
   core::ErrorObject *einst = static_cast<core::ErrorObject* >(vm->self().asObject());
   Falcon::Error *err = einst->getError();

   if ( err != 0 )
   {
      CoreString *cs = new CoreString;
      err->toString( *cs );
      vm->retval( cs );
   }
   else
      vm->retnil();
}

/*#
   @method heading Error
   @brief Creates a short textual representation of the error.

   This method will only render the essential informations of the error,
   without printing the stack trace and without checking for other
   sub errors in the @a Error.subErrors array.

   @see Error.toString
*/
FALCON_FUNC  Error_heading ( ::Falcon::VMachine *vm )
{
   ErrorObject *einst = static_cast<ErrorObject *>(vm->self().asObject());
   Falcon::Error *err = static_cast<Falcon::Error *>(einst->getUserData());

   if ( err != 0 )
   {
      CoreString *cs = new CoreString;
      err->heading( *cs );
      vm->retval( cs );
   }
   else
      vm->retnil();
}

/*#
   @method getSysErrDesc Error
   @brief returns system specific error description.
   @return System specific error description or nil if not available.

   If the error was generated by the underlying system (that is, if
   systemError > 0) returns a system and locale dependent error
   description. The description is obtained by querying the relevant
   OS error description API/SDK.

*/
FALCON_FUNC  Error_getSysErrDesc ( ::Falcon::VMachine *vm )
{
   ErrorObject *einst = static_cast<ErrorObject *>(vm->self().asObject());
   Falcon::Error *err = static_cast<Falcon::Error *>(einst->getUserData());

   if ( err != 0 )
   {
      String temp;
      ::Falcon::Sys::_describeError( err->systemError(), temp );
      vm->retval( temp );
   }
   else
      vm->retnil();
}


/*#
   @class SyntaxError
   @brief Syntax error descriptor.

   @ingroup errors
   @ingroup general_purpose

   @optparam code A numeric error code.
   @optparam description A textual description of the error code.
   @optparam extra A descriptive message explaining the error conditions.

   @from Error code, description, extra

   This errors are generated by the compiler or the assembler when a compilation fails.
   Usually, scripts won't receive this unless they are using the compiler to compile
   themselves modules on the fly.
*/
FALCON_FUNC  SyntaxError_init ( ::Falcon::VMachine *vm )
{
   ErrorObject *einst = static_cast<ErrorObject *>(vm->self().asObject());
   if( einst->getFalconData() == 0 )
      einst->setUserData( new Falcon::SyntaxError );

   Error_init( vm );
}


/*#
   @class GenericError
   @brief Generic undefined failure.
   @ingroup errors
   @ingroup general_purpose
   @optparam code A numeric error code.
   @optparam description A textual description of the error code.
   @optparam extra A descriptive message explaining the error conditions.
   @from Error code, description, extra

   This error reports a generic failure, usually not recoverable, which
   cannot be classified under a more specific class. Examples are failure
   in VM linking of deserialized objects, once the de-serialization is
   succesfull both on the stream side (correct load) and on the code side
   (correct conversion into Falcon items).
*/
FALCON_FUNC  GenericError_init ( ::Falcon::VMachine *vm )
{
   ErrorObject *einst = static_cast<ErrorObject *>(vm->self().asObject());
   if( einst->getFalconData() == 0 )
      einst->setUserData( new Falcon::GenericError );

   Error_init( vm );
}

/*#
   @class CodeError
   @brief VM and internal coded related error descriptor.
   @ingroup errors
   @ingroup general_purpose
   @optparam code A numeric error code.
   @optparam description A textual description of the error code.
   @optparam extra A descriptive message explaining the error conditions.
   @from Error code, description, extra

   This hard errors are usually generated by the VM when it finds some corruption of the code,
   or some illegal istruction parameter. Scripts can hardly receive it, if not as a notification
   of something bad happened to another controlled script.

*/
FALCON_FUNC  CodeError_init ( ::Falcon::VMachine *vm )
{
   ErrorObject *einst = static_cast<ErrorObject *>(vm->self().asObject());
   if( einst->getUserData() == 0 )
      einst->setUserData( new Falcon::CodeError );

   Error_init( vm );
}

/*#
   @class IoError
   @brief Error on I/O operations.
   @ingroup errors
   @ingroup general_purpose
   @optparam code A numeric error code.
   @optparam description A textual description of the error code.
   @optparam extra A descriptive message explaining the error conditions.
   @from Error code, description, extra

   This error is raised when an I/O operation fails on the underlying
   system stream. It may also be generated by functions accessing streams
   at high abstraction level, as the functions used to serialize items
   or to read XML files.
*/
FALCON_FUNC  IoError_init ( ::Falcon::VMachine *vm )
{
   ErrorObject *einst = static_cast<ErrorObject *>(vm->self().asObject());
   if( einst->getUserData() == 0 )
      einst->setUserData( new Falcon::IoError );

   Error_init( vm );
}

/*#
   @class TypeError
   @brief Type mismatch in a typed operation.
   @ingroup errors
   @ingroup general_purpose
   @optparam code A numeric error code.
   @optparam description A textual description of the error code.
   @optparam extra A descriptive message explaining the error conditions.
   @from Error code, description, extra

   This error is generated when some operations requiring items of a
   certain type fail because of type mismatch.
*/

FALCON_FUNC  TypeError_init ( ::Falcon::VMachine *vm )
{
   ErrorObject *einst = static_cast<ErrorObject *>(vm->self().asObject());
   if( einst->getUserData() == 0 )
      einst->setUserData( new Falcon::TypeError );

   Error_init( vm );
}

/*#
   @class AccessError
   @brief Error accessing an indexed item.
   @ingroup errors
   @ingroup general_purpose
   @optparam code A numeric error code.
   @optparam description A textual description of the error code.
   @optparam extra A descriptive message explaining the error conditions.
   @from Error code, description, extra

   This error is generated when an array was accessed beyond its size,
   or a key was not found in a dictionary, or a requested property was
   not found in an object.

   In addition, core, RTL and extension functions may raise this error when the
   semantic is appropriate (i.e. when providing them unexisting property
   names, or when requesting to access an unexisting range in an array).
*/
FALCON_FUNC  AccessError_init ( ::Falcon::VMachine *vm )
{
   ErrorObject *einst = static_cast<ErrorObject *>(vm->self().asObject());
   if( einst->getUserData() == 0 )
      einst->setUserData( new Falcon::AccessError );

   Error_init( vm );
}

/*#
   @class MathError
   @brief Mathematical calculation error.
   @ingroup errors
   @ingroup general_purpose
   @optparam code A numeric error code.
   @optparam description A textual description of the error code.
   @optparam extra A descriptive message explaining the error conditions.
   @from Error code, description, extra

   This class is generated when a mathematical operation caused an error;
   it may be a domain error, as trying to extract the square root of a negative number,
   an overflow error or a division by zero. The error code will detail what kind of
   math error has happened.
*/
FALCON_FUNC  MathError_init ( ::Falcon::VMachine *vm )
{
   ErrorObject *einst = static_cast<ErrorObject *>(vm->self().asObject());
   if( einst->getUserData() == 0 )
      einst->setUserData( new Falcon::MathError );

   Error_init( vm );
}

/*#
   @class ParamError
   @brief Incongruent paremeter error.
   @ingroup errors
   @ingroup general_purpose
   @optparam code A numeric error code.
   @optparam description A textual description of the error code.
   @optparam extra A descriptive message explaining the error conditions.
   @from Error code, description, extra

   This error is generated when a function is called with insufficient parameters,
   or with parameters of the wrong types; this error may also be raised if the function
   determines the parameters not to be valid for other reasons (i.e. if the function
   requires not just a string as a parameter, but also a string of a minimum length).

   This error usually indicates a problem in the script code. Normally, it is to be
   considered a runtime error that has to be resolved by fixing an incorrect script,
   which should pass correct parameter to the functions it calls.
*/
FALCON_FUNC  ParamError_init ( ::Falcon::VMachine *vm )
{
   ErrorObject *einst = static_cast<ErrorObject *>(vm->self().asObject());
   if( einst->getUserData() == 0 )
      einst->setUserData( new Falcon::ParamError );

   Error_init( vm );
}

/*#
   @class ParseError
   @brief Generic input parsing error.
   @ingroup errors
   @ingroup general_purpose
   @optparam code A numeric error code.
   @optparam description A textual description of the error code.
   @optparam extra A descriptive message explaining the error conditions.
   @from Error code, description, extra

   This error is generated when a parsing operation fails. It is used by RTL and
   extension modules when an input that should respect a given format is malformed.
   In example, the core module throws this error when the command line parser is
   told to search for a named parameter, but the command line parameters have been
   exhausted.

   It is also used by the mxml module to report problems while parsing XML files.

   Finally, this error is available for script to describe an error status due to
   invalid inputs.
*/
FALCON_FUNC  ParseError_init ( ::Falcon::VMachine *vm )
{
   ErrorObject *einst = static_cast<ErrorObject *>(vm->self().asObject());
   if( einst->getUserData() == 0 )
      einst->setUserData( new Falcon::ParseError );

   Error_init( vm );
}

/*#
   @class CloneError
   @brief Item cannot be cloned.
   @ingroup errors
   @ingroup general_purpose
   @optparam code A numeric error code.
   @optparam description A textual description of the error code.
   @optparam extra A descriptive message explaining the error conditions.
   @from Error code, description, extra

   Items containing external data, provided by extension modules or
   embedding application, must respect a "clone" protocol that allows
   to share, or duplicate, the inner data between different Falcon
   instances of the cloned item.

   If the inner core of Falcon item coming from a non-falcon source
   does not respect this protocol, the item cannot be cloned.

   When this error is raised, it is usually because the script
   tried to explicitly duplicate a "very special object" (for example,
   as an external resource handle created by a module).
*/
FALCON_FUNC  CloneError_init ( ::Falcon::VMachine *vm )
{
   ErrorObject *einst = static_cast<ErrorObject *>(vm->self().asObject());
   if( einst->getUserData() == 0 )
      einst->setUserData( new Falcon::CloneError);

   Error_init( vm );
}

/*#
   @class InterruptedError
   @brief Wait operation interrupted.
   @ingroup errors
   @ingroup general_purpose
   @optparam code A numeric error code.
   @optparam description A textual description of the error code.
   @optparam extra A descriptive message explaining the error conditions.
   @from Error code, description, extra

   This error is raised when a wait interrupt request has been received
   by the VM during a blocking wait.

   @see interrupt_protocol
*/
FALCON_FUNC  IntrruptedError_init ( ::Falcon::VMachine *vm )
{
   ErrorObject *einst = static_cast<ErrorObject *>(vm->self().asObject());
   if( einst->getUserData() == 0 )
      einst->setUserData( new Falcon::InterruptedError );

   Error_init( vm );
}

/*#
   @class MessageError
   @brief Error in the messaging system.
   @ingroup errors
   @ingroup general_purpose
   @optparam code A numeric error code.
   @optparam description A textual description of the error code.
   @optparam extra A descriptive message explaining the error conditions.
   @from Error code, description, extra

   This error is raised when a wait interrupt request has been received
   by the VM during a blocking wait.

   @see interrupt_protocol
*/
FALCON_FUNC  MessageError_init ( ::Falcon::VMachine *vm )
{
   ErrorObject *einst = static_cast<ErrorObject *>(vm->self().asObject());
   if( einst->getUserData() == 0 )
      einst->setUserData( new Falcon::MessageError );

   Error_init( vm );
}

/*#
   @class TableError
   @brief Error in Table class core operations.
   @ingroup errors
   @ingroup general_purpose
   @optparam code A numeric error code.
   @optparam description A textual description of the error code.
   @optparam extra A descriptive message explaining the error conditions.
   @from Error code, description, extra

   This error is raised when an logic or constraint error is found in
   access or when modifying tables. It is also the result for some common
   table operations in case of failure (i.e. @a Table.find).

   @see Table
*/
FALCON_FUNC  TableError_init ( ::Falcon::VMachine *vm )
{
   ErrorObject *einst = static_cast<ErrorObject *>(vm->self().asObject());
   if( einst->getUserData() == 0 )
      einst->setUserData( new Falcon::TableError );

   Error_init( vm );
}

}
}

/* end of error_ext.cpp */