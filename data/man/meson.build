configure_file(input: 'eovim.1.in',
               output: 'eovim.1',
               configuration: conf_data)

gzip = find_program('gzip', required: true)

custom_target('man', output : 'eovim.1.gz',
              command: [gzip, '-c', '-9', proj_build_root / 'data/man/eovim.1'],
              capture : true,
              install : true,
              install_dir : dir_man / 'man1')
