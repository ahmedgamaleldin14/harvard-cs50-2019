import os
from datetime import datetime

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    # build a free dictionary for lookup function to add extra elements
    dic = {}

    # sum the shares of each specific symbol
    rows = db.execute("SELECT symbol, SUM(shares) shares FROM history WHERE id=:id GROUP BY symbol", id=session["user_id"])
    another_rows = rows

    # check if the sum of shares is zero, meaning the bought stocks equal the sold ones, then delete it
    for i in range(len(another_rows)):
        if i == len(rows):
            break
        elif rows[i]["shares"] == 0:
            del rows[i]
            i -= 1

    # add the name, price, total price to each row
    for i in range(len(rows)):
        dic = lookup(rows[i]["symbol"])
        rows[i]["name"] = dic["name"]
        rows[i]["price"] = usd(dic["price"])
        rows[i]["total"] = usd(float(dic["price"]) * rows[i]["shares"])

    # calculate the current cash
    user_cash = db.execute("SELECT cash FROM users WHERE id=:id", id=session["user_id"])

    # send rows and cash to be accessed by Jinja in index.html
    return render_template("index.html", rows=rows, cash=usd(user_cash[0]["cash"]))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    if request.method == "POST":

        # Ensure a symbol is given
        if not request.form.get("symbol"):
            return apology("missing symbol", 400)

        # Ensure number of shares is given
        if not request.form.get("shares"):
            return apology("missing shares", 400)

        # Ensure that it is invalid symbol
        if not lookup(request.form.get("symbol")):
            return apology("invalid symbol", 400)

        # Ensure that shares is a number
        if not request.form.get("shares").isdigit():
            return apology("shares is not a number", 400)

        # create dictionary for symbol info.
        dic = {}
        dic = lookup(request.form.get("symbol"))

        # update cash based on the bought item
        user_cash = db.execute("SELECT cash FROM users WHERE id=:id", id=session["user_id"])
        item_price = dic["price"] * int(request.form.get("shares"))

        # Ensure that the user can afford the item
        if item_price > user_cash[0]["cash"]:
            return apology("cannot afford", 400)

        # update cash field in users table
        db.execute("UPDATE users SET cash=:cash WHERE id=:id", cash=user_cash[0]["cash"]-item_price, id=session["user_id"])

        # insert the purchase info. into history table
        db.execute("INSERT INTO history (id, symbol, price, shares, time) VALUES(:id, :symbol, :price, :shares, :time)",
                id=session["user_id"], symbol=request.form.get("symbol"), price=dic["price"],
                    shares=request.form.get("shares"), time=datetime.now().strftime("%d/%m/%Y %H:%M:%S"))

        flash("Bought")
        return redirect("/")

    else:
        return render_template("buy.html")

''''
@app.route("/check", methods=["GET"])
def check():
    """Return true if username available, else false, in JSON format"""
    return jsonify("TODO")
'''

@app.route("/history")
@login_required
def history():
    # build a free dictionary for lookup function to add extra elements
    dic = {}

    # sum the shares of each specific symbol
    rows = db.execute("SELECT symbol, shares, time FROM history WHERE id=:id", id=session["user_id"])

    # add current price to each row
    for i in range(len(rows)):
        dic = lookup(rows[i]["symbol"])
        rows[i]["price"] = usd(dic["price"])

    # send rows and cash to be accessed by Jinja in index.html
    return render_template("history.html", rows=rows)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return render_template("quote.html")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    if request.method == "POST":

        # Ensure a symbol is given
        if not request.form.get("symbol"):
            return apology("missing symbol", 400)

        # Ensure that it is invalid symbol
        if not lookup(request.form.get("symbol")):
            return apology("invalid symbol", 400)

        # Use lookup function to extract info. of a given symbol
        dic = {}
        dic = lookup(request.form.get("symbol"))

        # Send data to quoted.html to display a text
        return render_template("quoted.html", name=dic["name"], symbol=dic["symbol"], price=usd(dic["price"]))

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password") or not request.form.get("confirmation"):
            return apology("must provide password", 400)

        # Ensure the two passwords match
        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("passwords do not match", 400)

        # generate a hash password for security
        hash_pass = generate_password_hash(request.form.get("password"))

        # insert the new username and password into users database
        # INSERT returns none if a primary key was taken, otherwise returns the next primary key
        result = db.execute("INSERT INTO users (username, hash) VALUES(:username, :hash)", username=request.form.get("username"), hash=hash_pass)

        # check if the username exists
        if result == None:
            return apology("username already exists", 400)

        # Remember which user has logged in
        session["user_id"] = result

        # Redirect user to home page
        flash("You registered!")
        return render_template("login.html")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    rows = db.execute("SELECT symbol, SUM(shares) shares FROM history WHERE id=:id GROUP BY symbol", id=session["user_id"])

    if request.method == "POST":

        # Ensure a symbol is given
        if not request.form.get("symbol"):
            return apology("missing symbol", 400)

        # Ensure number of shares is given
        if not request.form.get("shares"):
            return apology("missing shares", 400)

        # Ensure that shares is a number
        if not request.form.get("shares").isdigit():
            return apology("shares is not a number", 400)

        # Ensure that the user has that many stocks to sell
        for i in range(len(rows)):
            if rows[i]["symbol"] == request.form.get("symbol"):
                if int(request.form.get("shares")) > rows[i]["shares"]:
                    return apology("too many shares")

        # calculate the current price in the stock
        dic = {}
        dic = lookup(request.form.get("symbol"))

        # insert the purchase info. into history table
        db.execute("INSERT INTO history (id, symbol, price, shares, time) VALUES(:id, :symbol, :price, :shares, :time)",
                id=session["user_id"], symbol=request.form.get("symbol"), price=dic["price"],
                    shares=-int(request.form.get("shares")), time=datetime.now().strftime("%d/%m/%Y %H:%M:%S"))

        # update cash based on the sold item
        user_cash = db.execute("SELECT cash FROM users WHERE id=:id", id=session["user_id"])
        item_price = dic["price"] * int(request.form.get("shares"))

        # update cash field in users table
        db.execute("UPDATE users SET cash=:cash WHERE id=:id", cash=user_cash[0]["cash"]+item_price, id=session["user_id"])

        flash("Sold")
        return redirect("/")

    else:
        # send the symbols to sell.html to be accessed by Jinja
        stock = []

        for i in range(len(rows)):
            if rows[i]["shares"] != 0:
                stock.append(rows[i]["symbol"])

        return render_template("sell.html", stocks=stock)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
