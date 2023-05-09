from flask import Flask, redirect, request, render_template, session, url_for
import sqlite3
import os
from datetime import datetime, time, timedelta
from decimal import Decimal
import platform

db = 'Items.db'


class Account:
    def __init__(self, id=None, firstName=None, lastName=None, phoneNumber=None, username=None, password=None, position=None, address=None, email=None, code=None):
        self.id = id
        self.firstName = firstName
        self.lastName = lastName
        self.phoneNumber = phoneNumber
        self.username = username
        self.password = password
        self.position = position
        self.address = address
        self.email = email
        self.code = code


class Item:
    def __init__(self, id=None, name=None, price=None, department=None, isPerPound=None, stock=None, sold=None):
        self.id = id
        self.name = name
        self.price = price
        self.department = department
        self.isPerPound = isPerPound
        self.stock = stock
        self.sold = sold


class Order:
    def __init__(self, id=None, items=None, total=None, pickupDate=None, status=None, notes=None, accountId=None):
        self.id = id
        self.items = items
        self.total = total
        self.pickupDate = pickupDate
        self.status = status
        self.notes = notes
        self.accountId = accountId


def select_account(username):
    conn = sqlite3.connect(db)
    c = conn.cursor()
    account_data = c.execute(
        'SELECT * FROM Accounts WHERE username = ?', (username,)
    ).fetchone()
    conn.close()
    if account_data:
        return Account(*account_data)
    else:
        return None


def select_item(id):
    conn = sqlite3.connect(db)
    c = conn.cursor()
    item_data = c.execute('SELECT * FROM Items WHERE id = ?', (id,)).fetchone()
    conn.close()
    return Item(*item_data)


def select_item_by_name(name):
    conn = sqlite3.connect(db)
    c = conn.cursor()
    item_data = c.execute(
        'SELECT * FROM Items WHERE name = ?', (name,)).fetchone()
    conn.close()
    return Item(*item_data)


def update_item(item):
    conn = sqlite3.connect(db)
    c = conn.cursor()
    c.execute(
        'Update Items SET stock = ?, sold = ? WHERE id = ?', (item.stock, item.sold, item.id,))
    conn.commit()
    conn.close()


def select_all_items(cart):
    conn = sqlite3.connect(db)
    c = conn.cursor()
    itemsList = c.execute(
        'SELECT * FROM ITEMS WHERE stock > 0'
    )
    items = []
    for row in itemsList:
        item = Item(*row)
        if str(item.id) in cart:
            item.stock -= float(cart[str(item.id)]['quantity'])
            if item.stock <= 0:
                continue
            else:
                items.append(item)
        else:
            items.append(item)
    conn.close()
    return items


def select_all_filtered_items(filter, keyword, cart):
    conn = sqlite3.connect(db)
    c = conn.cursor()
    itemsList = c.execute(
        'SELECT * FROM ITEMS WHERE stock > 0 AND ' +
        filter + ' = ?', (keyword,)
    )
    items = []
    for row in itemsList:
        item = Item(*row)
        if str(item.id) in cart:
            item.stock -= float(cart[str(item.id)]['quantity'])
            if item.stock <= 0:
                continue
            else:
                items.append(item)
        else:
            items.append(item)
    conn.close()
    return items


def select_all_orders_in_progress(accountId):
    conn = sqlite3.connect(db)
    c = conn.cursor()
    orderList = c.execute(
        "SELECT * FROM Orders WHERE status IN ('ordered', 'prepared') AND accountId=? ORDER BY pickupDate", (accountId,))
    orders = []
    for row in orderList:
        order = Order(
            id=row[0],
            items=row[1].split(','),
            total=row[2],
            pickupDate=row[3],
            status=row[4],
            notes=row[5],
            accountId=row[6]
        )
        if platform.system() == 'Windows':
            order.pickupDate = datetime.fromisoformat(
                order.pickupDate).strftime('%A, %B %e, %Y at %#I:%M %p')
        else:
            order.pickupDate = datetime.fromisoformat(
                order.pickupDate).strftime('%A, %B %e, %Y at %-I:%M %p')
        orders.append(order)
    return orders


def select_all_complete_orders(accountId):
    conn = sqlite3.connect(db)
    c = conn.cursor()
    orderList = c.execute(
        "SELECT * FROM Orders WHERE status IN ('complete') AND accountId=? ORDER BY pickupDate", (accountId,))
    orders = []
    for row in orderList:
        order = Order(
            id=row[0],
            items=row[1].split(','),
            total=row[2],
            pickupDate=row[3],
            status=row[4],
            notes=row[5],
            accountId=row[6]
        )
        if platform.system() == 'Windows':
            order.pickupDate = datetime.fromisoformat(
                order.pickupDate).strftime('%A, %B %e, %Y at %#I:%M %p')
        else:
            order.pickupDate = datetime.fromisoformat(
                order.pickupDate).strftime('%A, %B %e, %Y at %-I:%M %p')
        orders.append(order)
    return orders


def select_order(id):
    conn = sqlite3.connect(db)
    c = conn.cursor()
    order_data = c.execute(
        'SELECT * FROM Orders WHERE id = ?', (id,)).fetchone()
    conn.close()
    order = Order(
        id=order_data[0],
        items=order_data[1].split(','),
        total=order_data[2],
        pickupDate=order_data[3],
        status=order_data[4],
        notes=order_data[5],
        accountId=order_data[6]
    )
    if platform.system() == 'Windows':
        order.pickupDate = datetime.fromisoformat(
            order.pickupDate).strftime('%A, %B %e, %Y at %#I:%M %p')
    else:
        order.pickupDate = datetime.fromisoformat(
            order.pickupDate).strftime('%A, %B %e, %Y at %-I:%M %p')

    temp_dict = {}
    for item in order.items:
        if item in temp_dict:
            temp_dict[item] += 1
        else:
            temp_dict[item] = 1
    order.items = temp_dict
    return order


def updateAccount(account):
    conn = sqlite3.connect(db)
    c = conn.cursor()
    c.execute("UPDATE Accounts SET firstName = ?, lastName = ?, phoneNumber = ?, username = ?, password= ?, address = ?, email = ? WHERE id = ?",
              (account.firstName, account.lastName, account.phoneNumber, account.username, account.password, account.address, account.email, account.id,))
    conn.commit()
    conn.close()


def createAccount(account):
    conn = sqlite3.connect(db)
    c = conn.cursor()
    c.execute("INSERT INTO Accounts (firstName, lastName, phoneNumber, username, password, address, email) VALUES (?, ?, ?, ?, ?, ?, ?)",
              (account.firstName, account.lastName, account.phoneNumber, account.username, account.password, account.address, account.email))
    conn.commit()
    conn.close()


def createOrder(order):
    conn = sqlite3.connect(db)
    c = conn.cursor()
    c.execute(
        "INSERT INTO Orders (items, total, pickupDate, status, notes, accountId) VALUES (?, ?, ?, ?, ?, ?)", (order.items, order.total, order.pickupDate, order.status, order.notes, order.accountId))
    conn.commit()
    conn.close()


def cancelUserOrder(orderId):
    conn = sqlite3.connect(db)
    c = conn.cursor()
    c.execute(
        "DELETE FROM Orders WHERE id = ?", (orderId, ))
    conn.commit()
    conn.close()


app = Flask(__name__)
app.secret_key = "secret_key"


@app.route('/')
def login():
    return render_template('index.html')


@app.route('/kill')
# Development purposes only, remove before submitting
def kill():
    os._exit(0)


@app.route('/login', methods=['POST', 'GET'])
def login_submit_form():
    if request.method != 'POST':
        return redirect("/")
    username = request.form['username']
    password = request.form['password']
    account = select_account(username)
    if account and account.password == password:
        session['account'] = account.__dict__
        if 'cart' not in session:
            session['cart'] = {}
        return redirect('/store')
    else:
        return redirect(url_for('login', error='Invalid username or password'))


@app.route('/register')
def register():
    return render_template('register.html')


@app.route('/settings')
def settings():
    if 'account' in session:
        account = Account(**session.get('account'))
        return render_template('settings.html', account=account)
    return redirect('/')


@app.route('/updateAccount', methods=['POST', 'GET'])
def update_account_form():
    if request.method != 'POST':
        return redirect(url_for('settings'))
    account = Account()
    account.id = session['account']['id']
    account.username = request.form['username']
    account.password = request.form['password']
    account.phoneNumber = request.form['phoneNumber']
    account.firstName = request.form['firstName']
    account.lastName = request.form['lastName']
    account.address = request.form['address']
    account.email = request.form['email']
    if not session['account']['username'] == account.username:
        accountExist = select_account(account.username)
        if accountExist:
            return redirect(url_for('settings', error="Username already in use"))
    updateAccount(account)
    newAccount = select_account(account.username)
    session['account'] = newAccount.__dict__
    session.modified = True
    return redirect('/store')


@app.route('/register', methods=['POST'])
def register_submit_form():
    account = Account()
    account.username = request.form['username']
    account.password = request.form['password']
    account.phoneNumber = request.form['phoneNumber']
    account.firstName = request.form['firstName']
    account.lastName = request.form['lastName']
    account.address = request.form['address']
    account.email = request.form['email']
    accountExist = select_account(account.username)
    if not accountExist:
        createAccount(account)
        return redirect('/')
    return redirect(url_for('register', error='Username already in use'))


@app.route('/logout')
def logout():
    session.clear()
    return redirect('/')


@app.route('/addToCart', methods=['POST', 'GET'])
def add_to_cart():
    if request.method != 'POST':
        return redirect(url_for('store'))
    itemId = request.form['itemId']
    itemName = request.form['itemName']
    itemPrice = Decimal(request.form['itemPrice'])
    itemWeight = Decimal(request.form['weight'])
    isPerPound = bool(int(request.form['isPerPound']))
    if 'cart' not in session:
        session['cart'] = {}
    if itemId not in session['cart']:
        session['cart'][itemId] = {
            'name': itemName,
            'price': itemPrice,
            'quantity': itemWeight,
            'isPerPound': isPerPound
        }
    else:
        session['cart'][itemId]['quantity'] = Decimal(
            session['cart'][itemId]['quantity']) + itemWeight
    session.modified = True
    return redirect('/store')


@app.route('/store')
def store():
    if 'account' in session:
        items = select_all_items(session['cart'])
        return render_template('store.html', items=items)
    return redirect('/')


@app.route('/cart')
def cart():
    if 'account' in session:
        cart = session.get('cart', {})
        items = []
        subtotal = 0
        total = 0
        tax = 0
        for item_id, item_info in cart.items():
            name = item_info["name"]
            price = item_info["price"]
            quantity = item_info["quantity"]
            isPerPound = item_info["isPerPound"]
            total_item_price = Decimal(price) * Decimal(quantity)
            subtotal += total_item_price
            items.append((item_id, name, price, quantity,
                         isPerPound, total_item_price))
            tax = subtotal * Decimal(0.06)
            total = subtotal + tax
        tomorrow = (datetime.now() + timedelta(days=1)).strftime("%Y-%m-%d")
        advance = (datetime.now() + timedelta(days=8)).strftime("%Y-%m-%d")
        error = session.pop('error', None)
        notes = session.pop('notes', "")
        return render_template("cart.html", items=items, total=total, tax=tax, subtotal=subtotal, min=tomorrow, max=advance, error=error, notes=notes)
    return redirect('/')


@app.route('/removeItem', methods=['POST', 'GET'])
def removeItem():
    if request.method != 'POST':
        return redirect(url_for('cart'))
    if 'account' in session:
        itemId = request.form["id"]
        amount = Decimal(request.form["amount"])
        if itemId in session['cart']:
            session['cart'][itemId]['quantity'] = Decimal(
                session['cart'][itemId]['quantity']) - amount
            if session['cart'][itemId]['quantity'] == 0:
                del session['cart'][itemId]
        session.modified = True
        return redirect(url_for('cart'))
    return redirect('/')


@app.route('/checkout', methods=['POST', 'GET'])
def checkout():
    if request.method != 'POST':
        return redirect(url_for('cart'))
    if 'account' in session:
        cart = session.get('cart', {})
        if len(cart) == 0:
            return redirect(url_for('cart'))
        item_string = ""
        notes = request.form["notes"]
        pickup_date = request.form["pickup_date"]
        pickup_time = request.form["pickup_time"]
        pickup_datetime = datetime.strptime(
            pickup_date + " " + pickup_time, "%Y-%m-%d %H:%M")
        pickup_datetime = datetime.combine(
            pickup_datetime.date(), pickup_datetime.time()).isoformat()
        total = request.form["total"]
        outOfStock = ""
        removeFromCart = []
        itemsToUpdate = []
        for itemId, item_info in cart.items():
            name = item_info["name"]
            quantity = item_info["quantity"]
            item = select_item(itemId)
            if item.stock < float(quantity):
                removed = Decimal(
                    session['cart'][itemId]['quantity']) - Decimal(item.stock)
                session['cart'][itemId]['quantity'] = item.stock
                if (item.stock == 0):
                    removeFromCart.append(itemId)
                    session.modified = True
                outOfStock += name + " X " + str(removed) + ", "
            else:
                item.stock -= float(quantity)
                item.sold += float(quantity)
                itemsToUpdate.append(item)
            if item.isPerPound == True:
                item_string += str(quantity) + "lb " + name + ","
            else:
                for _ in range(int(quantity)):
                    item_string += name + ","
        if outOfStock == "":
            item_string = item_string[:-1]
            order = Order()
            order.items = item_string
            order.pickupDate = pickup_datetime
            order.total = total
            order.accountId = session['account']['id']
            order.status = "ordered"
            order.notes = notes
            for item in itemsToUpdate:
                update_item(item)
            createOrder(order)
            session['cart'] = {}
            session.modified = True
            return redirect("/orders")
        else:
            for id in removeFromCart:
                del session['cart'][id]
            session['error'] = outOfStock[:-2] + \
                " out of stock, item(s) removed from cart!"
            session["notes"] = notes
            return redirect(url_for('cart'))
    return redirect('/')


@app.route('/filterStore', methods=['POST', 'GET'])
def filterStore():
    if request.method != 'POST':
        return redirect(url_for('store'))
    action = request.form["submit"]
    filter = request.form["filter"]
    keyword = request.form["keyword"]
    if action == "Filter" and keyword != "":
        items = select_all_filtered_items(filter, keyword, session['cart'])
        return render_template('store.html', items=items)
    else:
        return redirect('/store')


@app.route('/orders')
def orders():
    if 'account' in session:
        currentOrders = select_all_orders_in_progress(session['account']['id'])
        previousOrders = select_all_complete_orders(session['account']['id'])
        return render_template('orders.html', currentOrders=currentOrders, previousOrders=previousOrders)
    return redirect('/')


@app.route('/orderDetails', methods=['GET', 'POST'])
def orderDetails():
    if 'account' in session:
        if request.method != 'POST':
            return redirect(url_for('orders'))
        orderId = request.form['orderId']
        order = select_order(orderId)
        return render_template('orderDetails.html', order=order)
    return redirect('/')


@app.route('/cancelOrder', methods=['GET', 'POST'])
def cancelOrder():
    if 'account' in session:
        if request.method != 'POST':
            return redirect(url_for('orders'))
        orderId = request.form['orderId']
        order = select_order(orderId)
        for key, value in order.items.items():
            if key[0].isdigit():
                item = key.split('lb ')[1]
                quantity = key.split('lb ')[0]
            else:
                item = key
                quantity = value
            itemToUpdate = select_item_by_name(item)
            itemToUpdate.stock += float(quantity)
            update_item(itemToUpdate)
        cancelUserOrder(orderId)
        return redirect(url_for('orders', message="Successfully canceled order"))
    return redirect('/')


@app.errorhandler(404)
def page_not_found(e):
    return render_template("error.html"), 404


@app.errorhandler(500)
def internal_Server_error(e):
    return render_template("error2.html"), 500


if __name__ == '__main__':
    app.run()
