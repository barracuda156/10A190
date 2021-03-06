/* File object interface (what's left of it -- see io.py) */

#ifndef Py_FILEOBJECT_H
#define Py_FILEOBJECT_H
#ifdef __cplusplus
extern "C" {
#endif

#define PY_STDIOTEXTMODE "b"

PyAPI_FUNC(PyObject *) PyFile_FromFd(int, char *, char *, int, char *, char *,
				     char *, int);
PyAPI_FUNC(PyObject *) PyFile_GetLine(PyObject *, int);
PyAPI_FUNC(int) PyFile_WriteObject(PyObject *, PyObject *, int);
PyAPI_FUNC(int) PyFile_WriteString(const char *, PyObject *);
PyAPI_FUNC(int) PyObject_AsFileDescriptor(PyObject *);
PyAPI_FUNC(char *) Py_UniversalNewlineFgets(char *, int, FILE*, PyObject *);

/* The default encoding used by the platform file system APIs
   If non-NULL, this is different than the default encoding for strings
*/
PyAPI_DATA(const char *) Py_FileSystemDefaultEncoding;
PyAPI_DATA(const int) Py_HasFileSystemDefaultEncoding;

/* Internal API

   The std printer acts as a preliminary sys.stderr until the new io
   infrastructure is in place. */
PyAPI_FUNC(PyObject *) PyFile_NewStdPrinter(int);
PyAPI_DATA(PyTypeObject) PyStdPrinter_Type;

#ifdef __cplusplus
}
#endif
#endif /* !Py_FILEOBJECT_H */
