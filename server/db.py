import MySQLdb
import logging
import json

def setupLogger():
	logging.basicConfig(filename='/var/www/api/log', format='%(asctime)s %(message)s', level=logging.DEBUG)

def connect():
	setupLogger()
	try:
		return MySQLdb.connect("localhost", "stUser", "strangerthings", "StrangerThings")
	except MySQLdb.Error as e:
		logging.error("MySQL error: {e}".format(e=e))
		exit(1)

def get():
	db = connect()
	c = db.cursor()
	query = "select value from singleQueue where key=%s"
	c.execute(query, ("strangerThings"))

	ret = ""
	for row in c.fetchall():
		ret = row[0]

	return ret

def set(value):
	db = connect()
	c = db.cursor()

	statement = "update singleQueue set value=%s where key=%s"
	c.execute(statement, (value, "strangerThings"))
	db.commit()

def dequeue(req):
	ret = get()
	set("")
	req.write(ret)

def enqueue(req, value):
	set(value)

def peek(req):
	req.write(get())
