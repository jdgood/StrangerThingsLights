import MySQLdb
import logging
import json

#def setupLogger():
#	logging.basicConfig(filename='/var/www/api/log', format='%(asctime)s %(message)s', level=logging.DEBUG)

def connect():
#	setupLogger()
	try:
		return MySQLdb.connect("localhost", "stUser", "strangerthings", "StrangerThings")
	except MySQLdb.Error as e:
#		logging.error("MySQL error: {e}".format(e=e))
		print("MySQL error: {e}".format(e=e))
		exit(1)


def get():
	db = connect()
	c = db.cursor()
	query = "select queueValue from singleQueue where queueKey='strangerThings'"
	c.execute(query)

	ret = ""
	for row in c.fetchall():
		ret = row[0]

	return ret

def set(value):
	db = connect()
	c = db.cursor()

	statement = "update singleQueue set queueValue=%s where queueKey='strangerThings'"
	c.execute(statement, (value,))
	db.commit()

def dequeue(req):
	ret = get()
	set("")
	req.write(ret)

def enqueue(req, value):
	set(value)

def peek(req):
	req.write(get())
