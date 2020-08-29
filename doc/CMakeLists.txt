set(DOXYGEN_IMAGE_PATH ${PROJECT_SOURCE_DIR}/doc)
doxygen_add_docs(docs
    src/
    ALL
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)
