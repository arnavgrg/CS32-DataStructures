class Investment {
public:
	Investment(string name, int price);
	virtual ~Investment(); //virtual destructor 
	string name() const;
	virtual bool fungible() const; //virtual function
	virtual string description() const=0; //pure virtual function
	double purchasePrice() const;
private:
	string m_name;
	int m_price;
};

Investment::Investment(string name, int price) {
	m_name = name;
	m_price = price;
}

Investment::~Investment(){}

string Investment::name() const {
	return m_name;
}

bool Investment::fungible() const {
	return false;
}

double Investment::purchasePrice() const {
	return m_price;
}

///////////////////////////////////////////////

class Painting: public Investment {
public:
	Painting(string name, int price);
	virtual ~Painting();
	virtual string description() const;
};

Painting::Painting(string name, int price) : Investment(name, price) {}

Painting::~Painting() {
	cout << "Destroying " << name() << ", a " << description() << endl;
}

string Painting::description() const {
	return "painting";
}

///////////////////////////////////////////////

class Stock: public Investment {
public:
	Stock(string name, int price, string ticker);
	virtual ~Stock();
	virtual bool fungible() const;
	virtual string description() const;
private:
	string m_ticker;
};

Stock::Stock(string name, int price, string ticker): Investment(name,price){
	m_ticker = ticker;
}

Stock::~Stock() {
	cout << "Destroying " << name() << ", a stock holding" << endl;
}

bool Stock::fungible() const {
	return true;
}

string Stock::description() const {
	string temp = "stock trading as " + m_ticker;
	return temp;
}

///////////////////////////////////////////////

class House: public Investment {
public:
	House(string name, int price);
	virtual ~House();
	virtual string description() const;
};

House::House(string name, int price) : Investment(name, price) {}

House::~House() {
	cout << "Destroying the " << description() << " " << name() << endl;
}

string House::description() const {
	return "house";
}
