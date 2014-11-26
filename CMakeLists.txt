CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

ADD_SUBDIRECTORY(dynamicbox_service)
ADD_SUBDIRECTORY(livebox-service)

ADD_DEPENDENCIES(livebox-service dynamicbox_service)
