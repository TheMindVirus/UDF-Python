echo "Installing Universal Driver Framework - Python C++ Interpreter Driver"
xcopy /s /y "Python" "%SystemRoot%\System32\drivers\UMDF\Python\" 
devcon install "UDF.inf" "root\UDF"
pause