import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash
import datetime

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

###############  INDEX METHOD  ############
@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # userRows = db.execute("SELECT * FROM portfolio WHERE userid = ?", session["user_id"])
    cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])

    # getting the cash value of whats in the users banks account at the moment
    cash = cash[0]['cash']

    # grouping and getting the rows within porfolio
    userRows = db.execute("SELECT symbol, SUM(shares), price, SUM(price) FROM portfolio WHERE userid = ? GROUP BY symbol", session["user_id"])
    total = cash


    # Iterating through the rows
    for row in userRows:
        total += row['SUM(shares)'] * row['price']
        row['price'] = usd(row['price'])
        row['SUM(price)'] = usd(row['SUM(price)'])
        # row['total'] = usd(total)

    return render_template("index.html", userRows=userRows, cash=usd(cash), total=usd(total))
#################   BUY METHOD  ##################
@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        # first checking if inputs exist
        if not request.form.get("symbol"):
            return apology("must provide symbol", 403)
        elif not request.form.get("shares"):
            return apology("must provide number of shares", 400)

        # now we must check if the symbol exists and if the number of shares is postive
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")
        symbolInfo = lookup(request.form.get("symbol"))

        if symbolInfo is None:
            return apology("invalid symbol", 400)


        # in helpers.py the symbol was set to all uppercase
        symbol = symbol.upper()

        # convert the shares into an int and calculate the price

        try:
            shares = int(shares)
            if shares < 0:
                return apology("must input a positive number of shares", 400)
        except ValueError:
            return apology("shares must be a numeric value", 400)
        priceShares = shares * symbolInfo['price']

        # time to try to find the user in the database
        userBalance = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        userBalance = userBalance[0]['cash']

        # trying to see of they have enough money to buy
        if userBalance < priceShares:
            return apology("insufficient funds", 403)

        bankTotal = userBalance - priceShares

        # update the users table with new banktotal
        db.execute("UPDATE users SET cash = ? WHERE id = ?", bankTotal, session["user_id"])

        # insert into portfolio
        date = datetime.datetime.now()
        db.execute("INSERT INTO portfolio (userid, symbol, shares, price, date) VALUES (?, ?, ?, ?, ?)",
           session["user_id"], symbol, shares, priceShares, date)

        flash("Bought!")
        return redirect("/")

    else:
       return render_template("buy.html")

#################   HISTORY METHOD  ##################
@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    userRows = db.execute("SELECT symbol, shares, price, date FROM portfolio WHERE userid = ?", session["user_id"])
    for row in userRows:
        row['price'] = usd(row['price'])
    return render_template("history.html", userRows=userRows)
#################   LOGIN METHOD  ##################
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
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


#################   LOGOUT METHOD  ##################
@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


#################   QUOTE METHOD  ##################
@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        #requiring an entry in textbox
        if not request.form.get("symbol"):
            return apology("must provide symbol", 400)

        # taking the symbol from the form and using the lookup method from helpers
        # returns name, price, and symbol
        symbol = request.form.get("symbol")
        symbolInfo = lookup (symbol)
        if symbolInfo == None:
            return apology("invalid symbol", 400)
        symbolInfo['price'] = usd(symbolInfo['price'])

        return render_template("quoted.html", symbolInfo=symbolInfo)

    else:
        return render_template("quote.html")


#################   REGISTER METHOD  ##################
@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        #checking for confirmation field
        elif not request.form.get("confirmation"):
            return apology("must confirm password", 400)

        # passwords have to match
        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("passwords do not match", 400)

        username = request.form.get("username")
        password = request.form.get("password")
        # we dont want to store actual password in database
        hashedPassword = generate_password_hash(password)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", username)

        # checking if the length of rows to see if its 0 (no other users)

        if len(rows)!=0:
            return apology("this username is already taken", 400)

        # table users already exists in database so we insert into table
        db.execute("INSERT INTO users (username, hash) VALUES (?,?)", username, hashedPassword)

        # we now need to store the user into the session of registering them
        rows = db.execute("SELECT * FROM users WHERE username = ?", username)
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")
    else:
        return render_template("register.html")


#################   SELL METHOD  ##################
@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":

        # making sure there are inputs
        if not request.form.get("symbol"):
            return apology("must provide symbol", 403)
        elif not request.form.get("shares"):
            return apology("must provide number of shares", 400)

        # getting information from the submitted form
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        # check for errors when form is requested
        shares = int(shares)
        if shares < 1:
            return apology("must input a positive number of shares", 400)

        symbolInfo = lookup(symbol)

        # making sure the symbol exists
        if symbolInfo is None:
            return apology("invalid symbol", 403)

        symbolRows = db.execute("SELECT * FROM portfolio WHERE symbol = ? AND userid = ?", symbol, session["user_id"])

        if len(symbolRows) < 1:
            return apology("User does not own these shares", 403)

        # checking if the user has more or equal to the shares they want to sell
        sharesCount = db.execute("SELECT SUM(shares) AS total_shares FROM portfolio WHERE symbol = ? AND userid = ? GROUP BY symbol", symbol, session["user_id"])

        if sharesCount:
            sharesCount = int(sharesCount[0]['total_shares'])
            if sharesCount < shares:
                return apology("Not enough shares to sell", 400)
        else:
            return apology("Not enough shares to sell", 400)

        # changing the database accordingly
        sellPrice = symbolInfo['price']
        priceShares = sellPrice * shares
        userBalance = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        userBalance = userBalance[0]['cash']

        bankTotal = userBalance - priceShares
        # update the users table with new banktotal
        db.execute("UPDATE users SET cash = ? WHERE id = ?", bankTotal, session["user_id"])

        # update the portfolio table by inserting the sold shares as a negative value
        date = datetime.datetime.now()
        shares = shares * -1
        db.execute("INSERT INTO portfolio (userid, symbol, shares, price, date) VALUES (?, ?, ?, ?, ?)",
                   session["user_id"], symbol, shares, sellPrice, date)

        flash("Sold!")
        return redirect("/")
    else:
        portfolio = db.execute("SELECT symbol FROM portfolio WHERE userid = ? GROUP BY symbol", session["user_id"])
        return render_template("sell.html", portfolio=portfolio)


#################   DEPOSIT METHOD  ##################
@app.route("/deposit", methods=["GET", "POST"])
@login_required
def deposit():
    """Deposit more money into account"""
    if request.method == "POST":
        if not request.form.get("deposit"):
            return apology("Must input amount to deposit", 403)

        deposit = request.form.get("deposit")
        deposit = float(deposit)
        # doing checks on deposit to make sure it is a postive number
        if deposit < 0:
            return apology("Must deposit a postive number", 403)

        # calling user cash to modify it
        userCash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        userCash = userCash[0]['cash']

        # adding to databse
        deposited = userCash + deposit
        db.execute("UPDATE users SET cash = ? WHERE id = ?", deposited, session["user_id"])


        flash("Deposited!")
        return redirect("/")
    else:
        return render_template("deposit.html")
