/*
** 2006 August 23
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
** Test extension for testing the sqlite3_auto_extension() function.
*/
#include "tcl.h"
#include "sqlite3ext.h"

#ifndef SQLITE_OMIT_LOAD_EXTENSION
#ifndef SQLITE_CORE
static SQLITE_EXTENSION_INIT1
#endif

/*
** The sqr() SQL function returns the square of its input value.
*/
static void sqrFunc(
  sqlite3_context *context,
  int argc,
  sqlite3_value **argv
){
  double r = sqlite3_value_double(argv[0]);
  sqlite3_result_double(context, r*r);
}

/*
** This is the entry point to register the extension for the sqr() function.
*/
static int sqr_init(
  sqlite3 *db, 
  char **pzErrMsg, 
  const sqlite3_api_routines *pApi
){
  SQLITE_EXTENSION_INIT2(pApi);
  sqlite3_create_function(db, "sqr", 1, SQLITE_ANY, 0, sqrFunc, 0, 0);
  return 0;
}

/*
** The cube() SQL function returns the cube of its input value.
*/
static void cubeFunc(
  sqlite3_context *context,
  int argc,
  sqlite3_value **argv
){
  double r = sqlite3_value_double(argv[0]);
  sqlite3_result_double(context, r*r*r);
}

/*
** This is the entry point to register the extension for the cube() function.
*/
static int cube_init(
  sqlite3 *db, 
  char **pzErrMsg, 
  const sqlite3_api_routines *pApi
){
  SQLITE_EXTENSION_INIT2(pApi);
  sqlite3_create_function(db, "cube", 1, SQLITE_ANY, 0, cubeFunc, 0, 0);
  return 0;
}

/*
** This is a broken extension entry point
*/
static int broken_init(
  sqlite3 *db, 
  char **pzErrMsg, 
  const sqlite3_api_routines *pApi
){
  char *zErr;
  SQLITE_EXTENSION_INIT2(pApi);
  zErr = sqlite3_mprintf("broken autoext!");
  *pzErrMsg = zErr;
  return 1;
}

/*
** tclcmd:   sqlite3_auto_extension_sqr
**
** Register the "sqr" extension to be loaded automatically.
*/
static int autoExtSqrObjCmd(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
  int rc = sqlite3_auto_extension((void*)sqr_init);
  Tcl_SetObjResult(interp, Tcl_NewIntObj(rc));
  return SQLITE_OK;
}

/*
** tclcmd:   sqlite3_auto_extension_cube
**
** Register the "cube" extension to be loaded automatically.
*/
static int autoExtCubeObjCmd(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
  int rc = sqlite3_auto_extension((void*)cube_init);
  Tcl_SetObjResult(interp, Tcl_NewIntObj(rc));
  return SQLITE_OK;
}

/*
** tclcmd:   sqlite3_auto_extension_broken
**
** Register the broken extension to be loaded automatically.
*/
static int autoExtBrokenObjCmd(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
  int rc = sqlite3_auto_extension((void*)broken_init);
  Tcl_SetObjResult(interp, Tcl_NewIntObj(rc));
  return SQLITE_OK;
}

#endif /* SQLITE_OMIT_LOAD_EXTENSION */


/*
** tclcmd:   sqlite3_reset_auto_extension
**
** Reset all auto-extensions
*/
static int resetAutoExtObjCmd(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
  sqlite3_reset_auto_extension();
  return SQLITE_OK;
}


/*
** This procedure registers the TCL procs defined in this file.
*/
int Sqlitetest_autoext_Init(Tcl_Interp *interp){
#ifndef SQLITE_OMIT_LOAD_EXTENSION
  Tcl_CreateObjCommand(interp, "sqlite3_auto_extension_sqr",
          autoExtSqrObjCmd, 0, 0);
  Tcl_CreateObjCommand(interp, "sqlite3_auto_extension_cube",
          autoExtCubeObjCmd, 0, 0);
  Tcl_CreateObjCommand(interp, "sqlite3_auto_extension_broken",
          autoExtBrokenObjCmd, 0, 0);
#endif
  Tcl_CreateObjCommand(interp, "sqlite3_reset_auto_extension",
          resetAutoExtObjCmd, 0, 0);
  return TCL_OK;
}
