project('eovim', ['c'],
        version: '0.2.0.99',
        default_options:[
          'warning_level=1',
          'c_std=gnu18',
          'b_lto=true',
          'optimization=2',
        ])
proj_vers = meson.project_version().split('.')
proj_ver_major = proj_vers[0]
proj_ver_minor = proj_vers[1]
proj_ver_patch = proj_vers[2]

fs = import('fs')
if fs.is_dir('.git')
  proj_git_hash  = run_command('git', 'rev-parse', '--short', 'HEAD',
                               check: false).stdout().strip()
else
  proj_git_hash = 'no'
endif

if proj_vers.length() > 3
  proj_ver_tweak = proj_vers[3]
else
  proj_ver_tweak = '0'
endif

conf_data = configuration_data()
conf_data.set_quoted('PROJECT_VERSION_MAJOR', proj_ver_major)
conf_data.set_quoted('PROJECT_VERSION_MINOR', proj_ver_minor)
conf_data.set_quoted('PROJECT_VERSION_PATCH', proj_ver_patch)
conf_data.set_quoted('PROJECT_VERSION_TWEAK', proj_ver_tweak)
if proj_git_hash == 'no'
  conf_data.set_quoted('EOVIM_VERSION', meson.project_version())
  conf_data.set('PROJECT_VERSION', meson.project_version())
else
  conf_data.set_quoted('EOVIM_VERSION', meson.project_version() + '-' + proj_git_hash)
  conf_data.set('PROJECT_VERSION', meson.project_version() + '-' + proj_git_hash)
endif

conf_data.set_quoted('EOVIM_VERSOIN_DATE', '2023-11-18')
conf_data.set_quoted('EOVIM_BUILD_TYPE', get_option('buildtype'))

# dir locations
dir_prefix = get_option('prefix')
dir_bin    = dir_prefix / get_option('bindir')
dir_lib    = dir_prefix / get_option('libdir')
dir_data   = dir_prefix / get_option('datadir')
dir_man    = dir_prefix / get_option('mandir')

proj_build_root = meson.project_build_root()
proj_src_root   = meson.project_source_root()

add_global_arguments('-D_GNU_SOURCE',
                     '-D_FORTIFY_SOURCE=2',
                     '-Wno-unused-parameter',
                     '-DSOURCE_DATA_DIR="../data"',
                     '-DBUILD_DATA_DIR="../data"',
                     '-DPACKAGE_BIN_DIR="'+dir_bin + '"',
                     '-DPACKAGE_LIB_DIR="'+dir_lib + '"',
                     '-DPACKAGE_DATA_DIR="'+dir_data + '/eovim"',
                     '-flto',
                     language:['c'])

inc_dir = include_directories('include')

efl_version = '>= 1.26.0'
elementary_dep = dependency('elementary', version: efl_version)
msgpack_dep = dependency('msgpack')

#ossldep      = dependency('openssl')
#expatdep     = dependency('expat')

subdir('data')
subdir('src')
executable('eovim', [src] , include_directories: inc_dir,
	    dependencies:[elementary_dep, msgpack_dep],
	    link_args: '-flto',
            install: true,
            install_dir : dir_bin)

run_target('cppcheck', command:['scripts/cppcheck.sh', '--project=' +
	   join_paths(meson.build_root(), 'compile_commands.json')])


configure_file(input: 'version.h.in',
               output: 'version.h',
               configuration: conf_data)
