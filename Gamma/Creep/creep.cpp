#include "data.h"


/*

int main(int argc, char* argv[])
{
	cout << endl << "** Creep v0.1 **" << endl;
	cout << "Copyright 1999 Peter J Durrant" << endl;
	cout << "email: P.J.Durrant@cs.cf.ac.uk" << endl;
	cout << "  URL: www.cs.cf.ac.uk/User/P.J.Durrant" << endl << endl;

	if (argc != 2) {
		cout << "Invalid command, required syntax:" << endl;
		cout << "creep <filename>" << endl << endl;
		exit(0);
	}

	ifstream ifs(argv[1]);
	if (!ifs) {
		cout << "Invalid filename" << endl;
		cout << "Please supply a valid filename" << endl << endl;
		exit(0);
	}
	ifs.close();

	try
	{
		RawData rawdata(argv[1]);
		Data data(rawdata);
		cout << data << endl;
	}
	catch (RawDataExcept& rde)
	{
		rde.Display();
	}
	catch (DataExcept& de)
	{
		de.Display();
	}
	catch (TransformExcept& te)
	{
		te.Display();
	}
	catch (ExCatch& e)
	{
		e.Display();
	}
	return 1;
}

*/
