
#include <iostream>
#include <map>
#include <set>
#include <string>

using namespace std;

typedef set< string > str_set;
typedef str_set::iterator str_set_p;

typedef pair< string, string > str_pair;

typedef map< str_pair, str_set > error_class_map;
typedef error_class_map::iterator error_class_map_p;

error_class_map Errors;

string LocalFile("LocalFile");
string RemoteFile("RemoteFile");
string File("File");
string Unix("Unix");
string Inet("Inet");
string Socket("Socket");
string LocalUnknown("LocalUnknown");
string RemoteUnknown("RemoteUnknown");

string LocalCopy("LocalCopy");
string RemoteCopy("RemoteCopy");
string LocalRead("LocalRead");
string RemoteRead("RemoteRead");
string FTPRead("FTPRead");
string LocalStore("LocalStore");
string RemoteStore("RemoteStore");
string LocalPipe("LocalPipe");
string RemotePipe("RemotePipe");
string FTPStore("FTPStore");

void set_value(string & first, string & second, string & value)
{
  Errors[str_pair(first, second)].insert(value);
}

void add_values(string & first, string & second,
		string & add_first, string & add_second)
{
  Errors[str_pair(first, second)].insert(Errors[str_pair(add_first, add_second)].begin(),
					 Errors[str_pair(add_first, add_second)].end());
}

void build_intermediate_for(string & buffer)
{

  // -- buffer --> File

  add_values(buffer, File, buffer, LocalFile);
  add_values(buffer, File, buffer, RemoteFile);

  // -- buffer --> Socket

  add_values(buffer, Socket, buffer, Unix);
  add_values(buffer, Socket, buffer, Inet);

  // -- buffer --> LocalUnknown

  add_values(buffer, LocalUnknown, buffer, LocalFile);
  add_values(buffer, LocalUnknown, buffer, Unix);

  // -- buffer --> RemoteUnknown

  add_values(buffer, RemoteUnknown, buffer, LocalUnknown);
  add_values(buffer, RemoteUnknown, buffer, RemoteFile);
  add_values(buffer, RemoteUnknown, buffer, Inet);
}

int main(int argc, char * argv[])
{
  // -- Set up the Top-level sets

  set_value(LocalFile, LocalFile, LocalCopy);
  set_value(LocalFile, RemoteFile, RemoteCopy);
  set_value(LocalFile, Unix, LocalRead);
  set_value(LocalFile, Inet, RemoteRead);

  set_value(RemoteFile, LocalFile, RemoteCopy);
  set_value(RemoteFile, RemoteFile, RemoteCopy);
  set_value(RemoteFile, Unix, LocalRead);
  set_value(RemoteFile, Inet, FTPRead);

  set_value(Unix, LocalFile, LocalStore);
  set_value(Unix, RemoteFile, LocalStore);
  set_value(Unix, Unix, LocalPipe);
  set_value(Unix, Inet, RemotePipe);

  set_value(Inet, LocalFile, RemoteStore);
  set_value(Inet, RemoteFile, FTPStore);
  set_value(Inet, Unix, RemotePipe);
  set_value(Inet, Inet, RemotePipe);

  // -- Populate the intermediate sets

  build_intermediate_for(LocalFile);
  build_intermediate_for(RemoteFile);
  build_intermediate_for(Unix);
  build_intermediate_for(Inet);

  // -- Build the File top-level sets

  add_values(File, LocalFile, LocalFile, LocalFile);
  add_values(File, LocalFile, RemoteFile, LocalFile);

  add_values(File, RemoteFile, LocalFile, RemoteFile);
  add_values(File, RemoteFile, RemoteFile, RemoteFile);

  add_values(File, Unix, LocalFile, Unix);
  add_values(File, Unix, RemoteFile, Unix);

  add_values(File, Inet, LocalFile, Inet);
  add_values(File, Inet, RemoteFile, Inet);

  // -- Populate the File intermediates

  build_intermediate_for(File);

  // -- Build the Socket top-level sets

  add_values(Socket, LocalFile, Unix, LocalFile);
  add_values(Socket, LocalFile, Inet, LocalFile);

  add_values(Socket, RemoteFile, Unix, RemoteFile);
  add_values(Socket, RemoteFile, Inet, RemoteFile);

  add_values(Socket, Unix, Unix, Unix);
  add_values(Socket, Unix, Inet, Unix);

  add_values(Socket, Inet, Unix, Inet);
  add_values(Socket, Inet, Inet, Inet);

  // -- Populate the Socket intermediates

  build_intermediate_for(Socket);

  // -- Build the LocalUnknown top-level sets

  add_values(LocalUnknown, LocalFile, LocalFile, LocalFile);
  add_values(LocalUnknown, LocalFile, Unix, LocalFile);

  add_values(LocalUnknown, RemoteFile, LocalFile, RemoteFile);
  add_values(LocalUnknown, RemoteFile, Unix, RemoteFile);

  add_values(LocalUnknown, Unix, LocalFile, Unix);
  add_values(LocalUnknown, Unix, Unix, Unix);

  add_values(LocalUnknown, Inet, LocalFile, Inet);
  add_values(LocalUnknown, Inet, Unix, Inet);

  // -- Populate the LocalUnknown intermediates

  build_intermediate_for(LocalUnknown);

  // -- Build the RemoteUnknown top-level sets

  add_values(LocalUnknown, LocalFile, LocalUnknown, LocalFile);
  add_values(LocalUnknown, LocalFile, RemoteFile, LocalFile);
  add_values(LocalUnknown, LocalFile, Inet, LocalFile);

  add_values(LocalUnknown, RemoteFile, LocalUnknown, RemoteFile);
  add_values(LocalUnknown, RemoteFile, RemoteFile, RemoteFile);
  add_values(LocalUnknown, RemoteFile, Inet, RemoteFile);

  add_values(LocalUnknown, Unix, LocalUnknown, Unix);
  add_values(LocalUnknown, Unix, RemoteFile, Unix);
  add_values(LocalUnknown, Unix, Inet, Unix);

  add_values(LocalUnknown, Inet, LocalUnknown, Inet);
  add_values(LocalUnknown, Inet, RemoteFile, Inet);
  add_values(LocalUnknown, Inet, Inet, Inet);

  // -- Populate the RemoteUnknown intermediates

  build_intermediate_for(RemoteUnknown);


  // -- Print the results

  for (error_class_map_p p = Errors.begin();
       p != Errors.end();
       ++p)
    {
      const str_pair & label = (*p).first;
      str_set & vals = (*p).second;

      for (str_set_p q = vals.begin();
	   q != vals.end();
	   ++q)
	{
	  const string & kind = *q;

	  cout << "  report if (";

	  if (label.first == LocalUnknown)
	    cout << "FDKind : buffer is-exactly Unknown  && Trust : buffer is-exactly Local && \\" << endl;
	  else
	    if (label.first == RemoteUnknown)
	      cout << "FDKind : buffer is-exactly Unknown  && Trust : buffer is-exactly Remote && \\" << endl;
	    else
	      cout << "FDKind : buffer is-exactly " << label.first << " && \\" << endl;

	  if (label.second == LocalUnknown)
	    cout << "             FDKind : IOHandle is-exactly Unknown  && Trust : IOHandle is-exactly Local) \\" << endl;
	  else
	    if (label.second == RemoteUnknown)
	      cout << "             FDKind : IOHandle is-exactly Unknown  && Trust : IOHandle is-exactly Remote) \\" << endl;
	    else
	      cout << "             FDKind : IOHandle is-exactly " << label.second << ") \\" << endl;

	  cout << "    \"RECORD " << kind << " " << vals.size() << " \" ++ @context ++ \"\\n\"; \\" << endl;
	}
    }
}
