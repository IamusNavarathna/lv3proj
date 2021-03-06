##################################################
# Falcon Programming Language
#
##################################################

#Engine specific define
add_definitions(-DFALCON_ENGINE_EXPORTS)

include_directories(
  ${TOP_INCLUDE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/core_module  
)

message(STATUS "Compiling Falcon core on ${CMAKE_SYSTEM_NAME}" )

#Specific system files
if(UNIX OR APPLE)
  set(SYS_SPECIFIC
    baton_posix.cpp
    dir_sys_unix.cpp
    fstream_sys_unix.cpp
    mt_posix.cpp
    stdstreams_unix.cpp
    time_sys_unix.cpp
    vm_sys_posix.cpp
    vfs_file_unix.cpp
    signals_posix.cpp
  )

  #for solaris system flavor, add sys_solaris.cpp
  if(CMAKE_SYSTEM_NAME STREQUAL "SunOS" )
    list(APPEND SYS_SPECIFIC sys_solaris.cpp)
  else()
    list(APPEND SYS_SPECIFIC sys_unix.cpp)
  endif()

  #DL is different for unix and mac
  if(APPLE)
    list(APPEND SYS_SPECIFIC 
      heap_bsd.cpp 
      dll_mac.cpp 
    )
  elseif(CMAKE_SYSTEM_NAME STREQUAL "BSD")
    list(APPEND SYS_SPECIFIC
      heap_bsd.cpp
      dll_dl.cpp 
    )
  else()
    list(APPEND SYS_SPECIFIC
      heap_posix.cpp
      dll_dl.cpp
    )
  endif()


elseif(WIN32)

  set(SYS_SPECIFIC
    baton_win.cpp
    dir_sys_win.cpp
    dll_win.cpp
    fstream_sys_win.cpp
    heap_win.cpp
    mt_win.cpp
    stdstreams_win.cpp
    sys_win.cpp
    time_sys_win.cpp
    vm_sys_win.cpp
    vfs_file_win.cpp
    #falcon_engine.rc
    signals_win.cpp
  )
endif()

#################

set(CORE_MODULE_SOURCES
  core_module/array_ext.cpp
  core_module/attrib_ext.cpp
  core_module/cmdlineparser.cpp
  core_module/complex_ext.cpp
  core_module/continuation_ext.cpp  
  core_module/core_module.cpp
  core_module/coroutine_ext.cpp
  core_module/dict_ext.cpp
  core_module/dir_ext.cpp
  core_module/envvars.cpp
  core_module/error_ext.cpp
  core_module/fal_include.cpp
  core_module/file_ext.cpp
  core_module/format_ext.cpp
  core_module/function_ext.cpp
  core_module/functional_ext.cpp
  core_module/gc_ext.cpp
  core_module/input.cpp
  core_module/inspect.cpp
  core_module/item_ext.cpp
  core_module/iterator_ext.cpp
  core_module/list_ext.cpp
  core_module/math_ext.cpp
  core_module/membuf_ext.cpp
  core_module/message_ext.cpp
  core_module/oob_ext.cpp
  core_module/pagedict_ext.cpp
  core_module/param_ext.cpp
  core_module/path_ext.cpp
  core_module/poopseq_ext.cpp
  core_module/print.cpp
  core_module/random.cpp
  core_module/seconds.cpp
  core_module/sequence_ext.cpp
  core_module/serialize.cpp
  core_module/set_ext.cpp
  core_module/string_ext.cpp
  core_module/state_ext.cpp
  core_module/stringstream_ext.cpp
  core_module/sys_ext.cpp
  core_module/table.cpp
  core_module/time_ext.cpp
  core_module/transcode_ext.cpp
  core_module/tokenizer_ext.cpp
  core_module/uri_ext.cpp
  core_module/vminfo_ext.cpp
  core_module/webhelp.cpp
)


add_library(falcon_engine SHARED
  attribmap.cpp
  autocstring.cpp
  autoucsstring.cpp
  autowstring.cpp
  base64.cpp
  basealloc.cpp
  cacheobject.cpp
  crobject.cpp
  compiler.cpp
  complex.cpp
  continuation.cpp
  corearray.cpp
  coreclass.cpp
  coredict.cpp
  corefunc.cpp
  coreobject.cpp
  coreslot.cpp
  coretable.cpp
  debug.cpp
  deptab.cpp
  error.cpp
  falcondata.cpp
  falconobject.cpp
  fassert.cpp
  filestat.cpp
  format.cpp
  garbageable.cpp
  gcalloc.cpp
  gencode.cpp
  generatorseq.cpp
  genericlist.cpp
  genericmap.cpp
  genericvector.cpp
  genhasm.cpp
  gentree.cpp
  globals.cpp
  intcomp.cpp
  item.cpp
  item_co.cpp
  itemarray.cpp
  itemlist.cpp
  itemserial.cpp
  itemset.cpp
  itemtraits.cpp
  iterator.cpp
  lineardict.cpp
  linemap.cpp
  livemodule.cpp
  ltree.cpp
  membuf.cpp
  memhash.cpp
  memory.cpp
  mempool.cpp
  modloader.cpp
  module.cpp
  modulecache.cpp
  pagedict.cpp
  path.cpp
  pcode.cpp
  poopseq.cpp
  proptable.cpp
  rampmode.cpp
  rangeseq.cpp
  reflectobject.cpp
  rosstream.cpp
  runtime.cpp
  sequence.cpp
  service.cpp
  smba.cpp
  src_lexer.cpp
  stackframe.cpp
  stream.cpp
  streambuffer.cpp
  string.cpp
  stringitem.cpp
  stringstream.cpp
  strtable.cpp
  symbol.cpp
  symtab.cpp
  syntree.cpp
  timestamp.cpp
  trace.cpp
  traits.cpp
  transcoding.cpp
  tokenizer.cpp
  uri.cpp
  vfsprovider.cpp
  vmcontext.cpp
  vm.cpp
  vmmaps.cpp
  vmmsg.cpp
  vm_run.cpp
  vmsema.cpp
  #
  src_parser.hpp
  src_parser.cpp
  #
  
  ${CORE_MODULE_SOURCES}
  ${SYS_SPECIFIC}
)

#In unix and mac we have to add extra libraries
if(UNIX OR APPLE)
  #also dl for linux and solaris
  # and RT only for solaris and non-bsd systems
  set(SYS_LIBS pthread m)
  if(APPLE)
    list(APPEND SYS_LIBS dl)
  else()
    IF(CMAKE_SYSTEM_NAME MATCHES ".*BSD")
    else()
      list(APPEND SYS_LIBS dl rt)
    endif()
  endif()
  target_link_libraries(falcon_engine ${SYS_LIBS})
endif()

install(TARGETS falcon_engine DESTINATION bin)
