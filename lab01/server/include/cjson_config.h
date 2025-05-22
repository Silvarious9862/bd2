#ifndef CJSON_CONFIG_H
#define CJSON_CONFIG_H

#if defined(__GNUC__) && (__GNUC__ >= 4)
    // Для GCC (4 и выше) задаем атрибут видимости
    #define CJSON_PUBLIC(type) type __attribute__ ((visibility("default")))
#else
    #define CJSON_PUBLIC(type) type
#endif

#endif // CJSON_CONFIG_H
