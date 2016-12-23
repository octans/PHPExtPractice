
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_dtracer.h"

/* If you declare any globals in php_dtracer.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(dtracer)
*/

/* True global resources - no need for thread safety here */
static int le_dtracer;

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("dtracer.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_dtracer_globals, dtracer_globals)
    STD_PHP_INI_ENTRY("dtracer.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_dtracer_globals, dtracer_globals)
PHP_INI_END()
*/
/* }}} */
static void zval_dump(zval *z, int level) {
    HashTable *myht;
    char *key;
    zval **pData;
    ulong index;
    int ret;

    switch (Z_TYPE_P(z)) {
        case IS_LONG:
            php_printf("%*style = long, value = %ld\n", level * 4, "", Z_LVAL_P(z));
            break;
        case IS_STRING:
            php_printf("%*style = string, value = \"%s\", len = %d\n", level * 4, "", Z_STRVAL_P(z), Z_STRLEN_P(z));
            break;
        case IS_DOUBLE:
            php_printf("%*style = double, value = %0.6f\n", level * 4, "", Z_DVAL_P(z));
            break;
        case IS_ARRAY:
            myht = Z_ARRVAL_P(z);
            zend_hash_internal_pointer_reset(myht);
            php_printf("%*stype = array, value = %s\n", level * 4, "", HASH_KEY_NON_EXISTANT != zend_hash_has_more_elements(myht) ? "" : "empty");
            while (HASH_KEY_NON_EXISTANT != (ret = zend_hash_get_current_key(myht, &key, &index, 0))) {
                if (HASH_KEY_IS_STRING == ret) {
                    php_printf("%*s key: \"%s\"", (level + 1) * 4, "", key);
                } else if (HASH_KEY_IS_LONG == ret) {
                    php_printf("%*s key: %ld", (level + 1) * 4, "", index);
                }
                ret = zend_hash_get_current_data(myht, &pData);
                zval_dump(*pData, level + 1);
                zend_hash_move_forward(myht);
            }
            zend_hash_internal_pointer_end(Z_ARRVAL_P(z));
            break;
        default:
            php_printf("unknown");
    }

}

PHP_FUNCTION(dtracer_var_dump) {
    zval *z;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &z) == FAILURE) {
        RETURN_NULL();
    }

    zval_dump(z, 0);

    RETURN_TRUE;
}

PHP_FUNCTION(confirm_dtracer_compiled)
{
	char *arg = NULL;
	int arg_len, len;
	char *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	len = spprintf(&strg, 0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "dtracer", arg);
	RETURN_STRINGL(strg, len, 0);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/


/* {{{ php_dtracer_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_dtracer_init_globals(zend_dtracer_globals *dtracer_globals)
{
	dtracer_globals->global_value = 0;
	dtracer_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(dtracer)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(dtracer)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(dtracer)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(dtracer)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(dtracer)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "dtracer support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* {{{ dtracer_functions[]
 *
 * Every user visible function must have an entry in dtracer_functions[].
 */
const zend_function_entry dtracer_functions[] = {
	PHP_FE(confirm_dtracer_compiled,	NULL)		/* For testing, remove later. */
    PHP_FE(dtracer_var_dump, NULL)
	PHP_FE_END	/* Must be the last line in dtracer_functions[] */
};
/* }}} */

/* {{{ dtracer_module_entry
 */
zend_module_entry dtracer_module_entry = {
	STANDARD_MODULE_HEADER,
	"dtracer",
	dtracer_functions,
	PHP_MINIT(dtracer),
	PHP_MSHUTDOWN(dtracer),
	PHP_RINIT(dtracer),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(dtracer),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(dtracer),
	PHP_DTRACER_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_DTRACER
ZEND_GET_MODULE(dtracer)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
