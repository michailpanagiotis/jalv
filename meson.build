# Copyright 2020-2022 David Robillard <d@drobilla.net>
# SPDX-License-Identifier: 0BSD OR ISC

project('jalv', ['c'],
        version: '1.6.8',
        license: 'ISC',
        meson_version: '>= 0.56.0',
        default_options: [
          'b_ndebug=if-release',
          'buildtype=release',
          'c_std=c99',
        ])

jalv_src_root = meson.current_source_dir()
major_version = meson.project_version().split('.')[0]
version_suffix = '@0@-@1@'.format(meson.project_name(), major_version)

#######################
# Compilers and Flags #
#######################

# Required tools
cc = meson.get_compiler('c')

# Set global warning flags
if get_option('strict') and not meson.is_subproject()
  subdir('meson/warnings')
endif

# Set global warning suppressions
subdir('meson/suppressions')
add_project_arguments(c_suppressions, language: ['c'])

#######################
# Common Dependencies #
#######################

m_dep = cc.find_library('m', required: false)

thread_dep = dependency('threads')

serd_dep = dependency('serd-0',
                      version: '>= 0.30.0',
                      fallback: ['serd', 'serd_dep'])

lv2_dep = dependency('lv2',
                     version: '>= 1.18.0',
                     fallback: ['lv2', 'lv2_dep'])

lilv_dep = dependency('lilv-0',
                      version: '>= 0.24.0',
                      fallback: ['lilv', 'lilv_dep'])

###########
# Drivers #
###########

jack_dep = dependency('jack',
                      version: '>= 0.120.0',
                      include_type: 'system',
                      required: true)

backend_sources = files('src/jack.c')
backend_dep = jack_dep

##########################
# Platform Configuration #
##########################

platform_defines = [
  '-DJALV_VERSION="@0@"'.format(meson.project_version()),
  '-DZIX_STATIC',
]

# Determine whether to use POSIX
no_posix = get_option('posix').disabled() or host_machine.system() == 'windows'
if no_posix
  platform_defines += ['-DJALV_NO_POSIX']
elif host_machine.system() == 'darwin'
  platform_defines += [
    '-D_DARWIN_C_SOURCE',
    '-D_POSIX_C_SOURCE=200809L',
    '-D_XOPEN_SOURCE=600',
  ]
else
  platform_defines += [
    '-D_BSD_SOURCE',
    '-D_DEFAULT_SOURCE',
    '-D_POSIX_C_SOURCE=200809L',
    '-D_XOPEN_SOURCE=600',
  ]
endif

# Check for platform features with the build system
if get_option('checks')
  platform_defines += [
    '-DJALV_NO_DEFAULT_CONFIG',
  ]

  if no_posix
    platform_defines += ['-DHAVE_FILENO=0']
    platform_defines += ['-DHAVE_ISATTY=0']
    platform_defines += ['-DHAVE_MLOCK=0']
    platform_defines += ['-DHAVE_POSIX_MEMALIGN=0']
    platform_defines += ['-DHAVE_SIGACTION=0']
  else
    fileno_code = '''#include <stdio.h>
int main(void) { return fileno(stdin); }'''

    isatty_code = '''#include <unistd.h>
int main(void) { return isatty(0); }'''

    mlock_code = '''#include <sys/mman.h>
int main(void) { return mlock(0, 0); }'''

    posix_memalign_code = '''#include <stdlib.h>
int main(void) { void* mem; posix_memalign(&mem, 8, 8); }'''

    sigaction_code = '''#include <signal.h>
int main(void) { return sigaction(SIGINT, 0, 0); }'''

    platform_defines += '-DHAVE_FILENO=@0@'.format(
      cc.compiles(fileno_code,
                  args: platform_defines,
                  name: 'fileno').to_int())

    platform_defines += '-DHAVE_ISATTY=@0@'.format(
      cc.compiles(isatty_code,
                  args: platform_defines,
                  name: 'isatty').to_int())

    platform_defines += '-DHAVE_MLOCK=@0@'.format(
      cc.compiles(mlock_code,
                  args: platform_defines,
                  name: 'mlock').to_int())

    platform_defines += '-DHAVE_POSIX_MEMALIGN=@0@'.format(
      cc.compiles(posix_memalign_code,
                  args: platform_defines,
                  name: 'posix_memalign').to_int())

    platform_defines += '-DHAVE_SIGACTION=@0@'.format(
      cc.compiles(sigaction_code,
                  args: platform_defines,
                  name: 'sigaction').to_int())
  endif

  jack_metadata_code = '''#include <jack/metadata.h>
int main(void) { return !!&jack_set_property; }'''

  jack_port_type_get_buffer_size_code = '''#include <jack/jack.h>
int main(void) { return !!&jack_port_type_get_buffer_size; }'''

  platform_defines += '-DHAVE_JACK_METADATA=@0@'.format(
    cc.compiles(jack_metadata_code,
                args: platform_defines,
                name: 'jack_metadata').to_int())

  platform_defines += '-DHAVE_JACK_PORT_TYPE_GET_BUFFER_SIZE=@0@'.format(
    cc.compiles(jack_port_type_get_buffer_size_code,
                args: platform_defines,
                name: 'jack_port_type_get_buffer_size').to_int())
endif

############
# Programs #
############

sources = backend_sources + files(
  'src/control.c',
  'src/jalv.c',
  'src/log.c',
  'src/lv2_evbuf.c',
  'src/state.c',
  'src/symap.c',
  'src/worker.c',
  'src/zix/allocator.c',
  'src/zix/ring.c',
)

common_dependencies = [
  backend_dep,
  lilv_dep,
  m_dep,
  serd_dep,
  thread_dep,
]

# CLI executable
executable(
  'jalv',
  sources + files('src/jalv_console.c'),
  c_args: c_suppressions + platform_defines,
  dependencies: common_dependencies,
  include_directories: include_directories('src'),
  install: true,
)


#################
# Documentation #
#################

subdir('doc')

#########
# Tests #
#########

if not get_option('tests').disabled()
  subdir('test')
endif

if not meson.is_subproject()
  summary('Install prefix', get_option('prefix'))
  summary('Executables', get_option('prefix') / get_option('bindir'))
  summary('Man pages', get_option('prefix') / get_option('mandir'))

  summary('Backend', backend_dep.name())
endif
