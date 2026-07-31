# OpenSSL libcrypto for afw_crypto (curl-style hand find module).
# Prefer linking only libcrypto; OpenSSL SSL is not required for EVP/HMAC.

if(OpenSSLCrypto_FOUND)
  return()
endif()

find_path(OpenSSLCrypto_INCLUDE_DIR openssl/evp.h)
find_library(OpenSSLCrypto_LIBRARY NAMES crypto)

if(NOT OpenSSLCrypto_INCLUDE_DIR OR NOT OpenSSLCrypto_LIBRARY)
  message(FATAL_ERROR
    "libcrypto (OpenSSL) not found; install libssl-dev / openssl-devel "
    "or disable the afw_crypto srcdir")
endif()

set(OpenSSLCrypto_FOUND TRUE)
set(OpenSSLCrypto_INCLUDE_DIRS ${OpenSSLCrypto_INCLUDE_DIR})
set(OpenSSLCrypto_LIBRARIES ${OpenSSLCrypto_LIBRARY})

add_library(OpenSSLCrypto::Crypto IMPORTED INTERFACE)
set_property(TARGET OpenSSLCrypto::Crypto PROPERTY
  INTERFACE_INCLUDE_DIRECTORIES "${OpenSSLCrypto_INCLUDE_DIRS}")
set_property(TARGET OpenSSLCrypto::Crypto PROPERTY
  INTERFACE_LINK_LIBRARIES "${OpenSSLCrypto_LIBRARIES}")
