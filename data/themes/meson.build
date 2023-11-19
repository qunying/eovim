theme_src = [
  'default.edc',
  'cursor.edc',
  'cmdline.edc',
  'completion.edc',
  'tab.edc'
]
subdir('img')
subdir('snd')

edje_cc = find_program('edje_cc', required: true)

custom_target('theme', output: 'default.edj',
              command: [edje_cc, proj_src_root / 'data/themes/default.edc',
                       '@OUTPUT@',
                       '-author', proj_src_root / 'AUTHORS',
                       '-license', proj_src_root / 'LICENSE',
                       '-id', proj_src_root / 'data/themes/img',
                       '-sd', proj_src_root / 'data/themes/snd'],
              depend_files : theme_src,
              install : true,
              install_dir : dir_data / 'eovim/themes')

