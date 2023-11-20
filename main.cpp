#include <iostream>
#include <pqxx/pqxx>
#include <Windows.h>
//#pragma execution_character_set("utf-8")

class dt {
private:
	pqxx::connection conn{ ("dbname=forneto host=127.0.0.1"
		" port=5432 "
		"user=postgres password=123") };
	pqxx::work w{conn};
public:
	dt() {
	}

	void create_table_clients( ) {
		w.exec("CREATE TABLE IF NOT EXISTS clients ( id_client SERIAL PRIMARY KEY, fio VARCHAR(30) UNIQUE NOT NULL, email VARCHAR(30))");

		std::cout << "Table \"clients\" is created.\n\n";
	}

	void create_table_phones() {
		w.exec("CREATE TABLE IF NOT EXISTS phones ( id_phone SERIAL PRIMARY KEY, id_client INT, phone VARCHAR(30) UNIQUE)");

		std::cout << "Table \"phones\" is created.\n\n";
	}

	void create_client(std::string fio, std::string email) {
		w.exec("INSERT INTO clients(fio, email) "
			"VALUES('" + w.esc(fio) + "', '" + w.esc(email) + "')");

		std::cout << "Client with name: " << fio << " is created.\n\n";
	}

	void create_phone(std::string id_client, std::string const phone) {
		w.exec("INSERT INTO phones( phone, id_client) "
			"VALUES('" + w.esc(phone) + "',"+ w.esc(id_client) +")");

		std::cout << "Phone \"" << phone << "\" is created for client "<< id_client<< ".\n\n";
	}

	void update_client(std::string id, std::string fio, std::string email) {
		w.exec("UPDATE clients SET fio = '" + w.esc(fio) + "', email = '" + w.esc(email) + "' WHERE id_client = " + w.esc(id));

		std::cout << "Client " << id << " is updated.\n\n";
	}

	void delete_client(std::string id) {
		w.exec("DELETE FROM clients WHERE id_client = " + w.esc(id));

		std::cout << "Client " << id << " is deleted.\n\n";
	}

	void delete_phone(std::string id) {
		w.exec("DELETE FROM phones WHERE id_client = " + w.esc(id));

		std::cout << "Client " << id << "' phones are deleted.\n\n";
	}

	void find_client(std::string fe) {
		std::string fio = w.query_value<std::string>("SELECT fio FROM clients WHERE fio like '%" + w.esc(fe) + "%' OR email = '" + w.esc(fe) + "'");
		std::string email = w.query_value<std::string>("SELECT email FROM clients WHERE fio like '%" + w.esc(fe) + "%' OR email = '" + w.esc(fe) + "'");
		std::cout << "Client's name is: " << fio << ".\n";
		std::cout << "Client's email is: " << email << ".\n";
		for (auto [phone] : w.query<std::string>(
			"SELECT phone FROM phones JOIN clients ON phones.id_client=clients.id_client WHERE fio like '%" + w.esc(fe) + "%' OR email = '" + w.esc(fe) + "'"))
		{
			std::cout << "Client's phone is: " << phone << ".\n";
		}
		std::cout << "\n";
	}

	void find_client_with_phone(std::string f) {
		std::string fio = w.query_value<std::string>("SELECT fio FROM phones JOIN clients ON phones.id_client=clients.id_client"
			" WHERE phone = '" + w.esc(f) + "'");
		std::string email = w.query_value<std::string>("SELECT email FROM phones JOIN clients ON phones.id_client=clients.id_client"
			" WHERE phone = '" + w.esc(f) + "'");
		std::cout << "Client's name is: " << fio << ".\n";
		std::cout << "Client's email is: " << email << ".\n";
		for (auto [phone] : w.query<std::string>("SELECT phone FROM phones WHERE phone = '" + w.esc(f) + "'"))
		{
			std::cout << "Client's phone is: " << phone << ".\n";
		}
		std::cout << "\n";
	}

	void delete_table(std::string table){
		w.exec("DROP TABLE IF EXISTS " + w.esc(table));

		std::cout << "Table \"" << table << "\" is deleted.\n\n";
	}

	~dt() {
		w.commit();
	}
};


int main() {
	setlocale(LC_ALL, "Russian");
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
	//setvbuf(stdout,nullptr,_IOFBF,1000);

	std::cout << "This database contsins client's names, emails and pfones." << std::endl;

	try {


		dt DATA_BASE;

		DATA_BASE.delete_table("clients");
		DATA_BASE.delete_table("phones");

		DATA_BASE.create_table_clients();
		DATA_BASE.create_table_phones();

		DATA_BASE.create_client("john joe", "sdsdsd@gmail.com");
		DATA_BASE.create_phone("1","89346666463");

		DATA_BASE.find_client("john joe");

		DATA_BASE.update_client("1", "john kia", "12@gmail.com");

		DATA_BASE.find_client_with_phone("89346666463");

		DATA_BASE.delete_client("1");
		DATA_BASE.delete_phone("1");

	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}