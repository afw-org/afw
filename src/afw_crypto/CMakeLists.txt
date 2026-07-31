# Adaptive Framework afw_crypto extension (OpenSSL libcrypto)

# Include afwdev generated variables.
include(generated/afwdev_generated_variables.cmake)

# Include basic afw project.
include(generated/basic_afw_project.cmake)

# OpenSSL libcrypto (hand find module, same pattern as CURLConfig.cmake).
find_package(OpenSSLCrypto NO_MODULE PATHS cmake REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenSSLCrypto::Crypto)

# Include basic afw install.
include(generated/basic_afw_install.cmake)
