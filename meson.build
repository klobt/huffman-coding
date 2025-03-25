project('huffman_coding', 'c', version : '0.1', default_options : ['c_std=c99', 'default_library=static'])

add_project_arguments('-Werror', '-g', language: 'c')

lib_sources = [
        'binary.c',
        'char_array.c',
        'node.c',
        'node_array.c',
        'node_breadcrumb_array.c',
        'node_queue.c',
        'utilities.c',
]

libhuffman = static_library('huffman_lib', lib_sources)

encode_sources = ['encode.c']
decode_sources = ['decode.c']

huffman_encode = executable('huffman_encode', encode_sources, link_with : libhuffman)
huffman_decode = executable('huffman_decode', decode_sources, link_with : libhuffman)