MESSAGE = "[UDF]: Hello World"
LOGFILE = "C:\\Windows\\UDF-python.txt"

print(MESSAGE)
with open(LOGFILE, "w") as file:
    file.seek(0)
    file.write(MESSAGE)
    file.truncate()