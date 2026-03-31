//
// Created by Vitor Betmann on 2026-03-31.
//

/*
 * GenScene is a cli tool to generate scenes
 *
 * users should run build/GenState and pass in flags to automatically create boilerplate scene source and header files
 * ex: `smile/build/GenScene menu` should create src and header files for a scene called menu under the `src` or
 * `source` and the `include` directories respectively
 *
 * optional flags are:
 * --no-[funcName]: which creates template files without a given function
 *   funcName options: [ enter, update, draw, exit ]
 *   alias: [ ne, nu, nd, nx ]
 *
 * ex: smile/build/GenScene menu --no-enter -nx
 *
 * --[filetype]-in: which creates the source or header file inside a specific directory, overriding the default `src`
 * and `include`. This flag requires an additional argument
 * filetype options: [ source, header ]
 * alias: [ src, hdr]
 *
 * ex: smile/build/GenScene menu --source-in . -hdr my_header_files
 *
 * --help: brings up the help menu
 * alias: [ -h, <calling GenScene with no args> ]
 */

int main(int argc, char *argv[]) {
}
