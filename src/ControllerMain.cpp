///////////////////////////////////////////////////////////////////////////////
// ControllerMain.cpp
// ==================
// Derived Controller class for main window
///////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <commctrl.h>                   // common controls
#include <sstream>
#include "ControllerMain.h"
#include "resource.h"
#include "Log.h"
#include "sqlite3.h"
#include "string"
#include <vector>
#include <iostream>
#include "algebra_structs.h"
using namespace Win;
using std::cout;
using std::endl;

// handle events(messages) on all child windows that belong to the parent window.
// For example, close all child windows when the parent got WM_CLOSE message.
// lParam can be used to specify a event or message.
bool CALLBACK enumerateChildren(HWND childHandle, LPARAM lParam);



ControllerMain::ControllerMain(ModelGL* _model) : model(_model), glHandle(0), formHandle(0)
{
}

string oldType;

int callback(void *NotUsed, int argc, char **argv, char **azColName) {
	oldType = string(argv[0]);
	for (int i = 0; i<argc; i++) {
		printf("%s : %s\n", azColName[i], argv[i]);
	}
	printf("\n");
	return 0;
}

using Record = std::vector<std::string>;
using Records = std::vector<Record>;
sqlite3 *db;

int select_callback(void *p_data, int num_fields, char **p_fields, char **p_col_names)
{
	Records* records = static_cast<Records*>(p_data);
	try {
		records->emplace_back(p_fields, p_fields + num_fields);
	}
	catch (...) {
		// abort select on failure, don't let exception propogate thru sqlite3 call-stack
		return 1;
	}
	return 0;
}

Records select_stmt(const char* stmt)
{
	Records records;
	char *errmsg;
	int ret = sqlite3_exec(db, stmt, select_callback, &records, &errmsg);
	if (ret != SQLITE_OK) {
		std::cerr << "Error in select statement " << stmt << "[" << errmsg << "]\n";
	}
	else {
		std::cerr << records.size() << " records returned.\n";
	}

	return records;
}

int ControllerMain::command(int id, int cmd, LPARAM msg)
{
	Shape* nShape;
	char *zErrMsg = 0;
	const char* data = "Callback function called";
	int db_ID;
	string OBJECTS;
	string FACETS;
	string CENTER_COLOR_ID;
	string QUERY;
	Records records;
	Records records1;

	string type;
	float3 center;
	float3 colour;
	GLuint ID;
	vector<float3> verteces;
	vector<float3> normals;
	vector<facet> facets;

	float verx, very, verz;
	int vx, vy, vz, nx, ny, nz;
	string::size_type sz;
	string buff;

	switch (id)
	{
	case ID_CUBOID:
		nShape = new Cuboid(1, 1, 1);
		model->shapes.push_back(nShape);
		break;
	case ID_TETRAEDR:
		nShape = new Tetraedr(1);
		model->shapes.push_back(nShape);
		break;
	case ID_FILE_EXIT:
		break;
	case ID_HIDE_GRID:
		model->hide_grid();
		break;
	//case ID_FILE_NEW:
	//	break;
	case ID_FILE_OPEN:
	{
		db_ID = sqlite3_open("shapes.db", &db);
		if (db_ID) throw runtime_error("Can't open data base");

		model->shapes.clear();


		records = select_stmt("SELECT VAL FROM CENTER_COLOR_ID");
		buff = records.at(0).at(0);

		if (!buff.empty())
		{
			while (buff[0] != ' ')
			{
				type += buff[0];
				buff = buff.substr(1);
			}

			center.x = stof(buff, &sz);
			buff = buff.substr(sz);

			center.y = stof(buff, &sz);
			buff = buff.substr(sz);

			center.z = stof(buff, &sz);
			buff = buff.substr(sz);

			colour.x = stof(buff, &sz);
			buff = buff.substr(sz);

			colour.y = stof(buff, &sz);
			buff = buff.substr(sz);

			colour.z = stof(buff, &sz);
			buff = buff.substr(sz);

			ID = stoi(buff, &sz);
			buff = buff.substr(sz);
		}

		records = select_stmt("SELECT VERTECES FROM OBJECTS");
		for (int i = 0; records[i].at(0) != string("###"); i++)
		{
			buff = records[i].at(0);

			verx = stof(buff, &sz);
			buff = buff.substr(sz);

			very = stof(buff, &sz);
			buff = buff.substr(sz);

			verz = stof(buff, &sz);
			buff = buff.substr(sz);
			verteces.push_back(float3(verx, very, verz));
			verx = very = verz = sz = 0;
		}

		records = select_stmt("SELECT NORMALS FROM OBJECTS");
		for (int i = 0; records[i].at(0) != string("###"); i++)
		{
			buff = records[i].at(0);

			verx = stof(buff, &sz);
			buff = buff.substr(sz);

			very = stof(buff, &sz);
			buff = buff.substr(sz);

			verz = stof(buff, &sz);
			buff = buff.substr(sz);

			normals.push_back(float3(verx, very, verz));
		}

		records = select_stmt("SELECT TRIENGLES FROM FACETS");
		records1 = select_stmt("SELECT NORMALS FROM FACETS");
		for (int i = 0; records[i].at(0) != string("###"); i++)
		{
			buff = records[i].at(0);

			vx = stoi(buff, &sz);
			buff = buff.substr(sz);

			vy = stoi(buff, &sz);
			buff = buff.substr(sz);

			vz = stoi(buff, &sz);
			buff = buff.substr(sz);

			buff = records1[i].at(0);

			nx = stoi(buff, &sz);
			buff = buff.substr(sz);

			ny = stoi(buff, &sz);
			buff = buff.substr(sz);

			nz = stoi(buff, &sz);
			buff = buff.substr(sz);

			facets.push_back(facet(vx, vy, vz, nx, ny, nz));
		}
		if (type == string("Cuboid"))
		{
			model->shapes.push_back(new Cuboid(center, colour, ID, verteces, normals, facets));
		}
		else model->shapes.push_back(new Tetraedr(center, colour, ID, verteces, normals, facets));
		sqlite3_close(db);
	}
		break;
	case ID_FILE_SAVE:
	{
		db_ID = sqlite3_open("shapes.db", &db);
		if (db_ID) throw runtime_error("Can't open data base");
		OBJECTS = "CREATE TABLE IF NOT EXISTS OBJECTS (VERTECES TEXT, NORMALS TEXT);";
		FACETS = "CREATE TABLE IF NOT EXISTS FACETS (TRIENGLES TEXT, NORMALS TEXT);";
		CENTER_COLOR_ID = "CREATE TABLE IF NOT EXISTS CENTER_COLOR_ID (VAL TEXT);";
		db_ID = sqlite3_exec(db, OBJECTS.c_str(), callback, 0, &zErrMsg);
		db_ID = sqlite3_exec(db, FACETS.c_str(), callback, 0, &zErrMsg);
		db_ID = sqlite3_exec(db, CENTER_COLOR_ID.c_str(), callback, 0, &zErrMsg);

		QUERY = "DELETE FROM CENTER_COLOR_ID;";
		sqlite3_exec(db, QUERY.c_str(), callback, 0, &zErrMsg);

		QUERY = "DELETE FROM OBJECTS;";
		sqlite3_exec(db, QUERY.c_str(), callback, 0, &zErrMsg);

		QUERY = "DELETE FROM FACETS;";
		sqlite3_exec(db, QUERY.c_str(), callback, 0, &zErrMsg);
		QUERY.clear();
		for (auto i : model->shapes)
		{
			for (int j = 0; j < i->verteces.size(); j++)
			{
				QUERY += "INSERT INTO OBJECTS (VERTECES, NORMALS) VALUES(";
				QUERY += "'" + to_string(i->verteces[j].x) + " " + to_string(i->verteces[j].y) + " " + to_string(i->verteces[j].z) + "'";
				QUERY += ", ";
				QUERY += "'" + to_string(i->normals[j].x) + " " + to_string(i->normals[j].y) + " " + to_string(i->normals[j].z) + "'";
				QUERY += ");";
				sqlite3_exec(db, QUERY.c_str(), callback, 0, &zErrMsg);
				QUERY.clear();
			}
			for (int j = 0; j < i->facets.size(); j++)
			{
				QUERY += "INSERT INTO FACETS (TRIENGLES, NORMALS) VALUES(";
				QUERY += "'" + to_string(i->facets[j].triangle[0]) + " " + to_string(i->facets[j].triangle[1]) + " " + to_string(i->facets[j].triangle[2]) + "'";
				QUERY += ", ";
				QUERY += "'" + to_string(i->facets[j].normal[0]) + " " + to_string(i->facets[j].normal[1]) + " " + to_string(i->facets[j].normal[2]) + "'";
				QUERY += ");";
				sqlite3_exec(db, QUERY.c_str(), callback, 0, &zErrMsg);
				QUERY.clear();
			}
			QUERY += "INSERT INTO CENTER_COLOR_ID VALUES(";
			QUERY += "'" + string(i->type()) + " " +
				to_string(i->center.x) + " " +
				to_string(i->center.y) + " " +
				to_string(i->center.z) + " " +
				to_string(i->colour.x) + " " +
				to_string(i->colour.y) + " " +
				to_string(i->colour.z) + " " +
				to_string(i->ID) + "'" + ");";
			sqlite3_exec(db, QUERY.c_str(), callback, 0, &zErrMsg);

			QUERY = "INSERT INTO OBJECTS (VERTECES, NORMALS) VALUES('###', '###');";
			sqlite3_exec(db, QUERY.c_str(), callback, 0, &zErrMsg);

			QUERY = "INSERT INTO FACETS (TRIENGLES, NORMALS) VALUES('###', '###');";
			sqlite3_exec(db, QUERY.c_str(), callback, 0, &zErrMsg);

			QUERY = "INSERT INTO CENTER_COLOR_ID (VAL) VALUES('###');";
			sqlite3_exec(db, QUERY.c_str(), callback, 0, &zErrMsg);
			QUERY.clear();
		}
		sqlite3_close(db);
	}
		break;
	//case ID_EDIT_CUT:
	//	//model->cut
	//	break;
	//case ID_EDIT_COPY:
	//	//model->cut
	//	break;
	//case ID_EDIT_PASTE:
	//	//model->cut
	//	break;
	}
    return 0;
}

int ControllerMain::close()
{
    Win::log(""); // blank line
    Win::log("Terminating application...");

    // close all child windows first
    ::EnumChildWindows(handle, (WNDENUMPROC)enumerateChildren, (LPARAM)WM_CLOSE);

    ::DestroyWindow(handle);    // close itself
    return 0;
}

int ControllerMain::destroy()
{
    ::PostQuitMessage(0);       // exit the message loop
 
    Win::log("Main window is destroyed.");
    return 0;
}

int ControllerMain::create()
{
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// handle WM_SIZE
// the width and height are for the client area
///////////////////////////////////////////////////////////////////////////////
int ControllerMain::size(int width, int height, WPARAM wParam)
{
    RECT rect;

    // get height of status bar
    HWND statusHandle = ::GetDlgItem(handle, IDC_STATUSBAR);
    ::GetWindowRect(statusHandle, &rect);
    int statusHeight = rect.bottom - rect.top;

    // get height of glDialog
    ::GetWindowRect(formHandle, &rect);
    int formHeight = rect.bottom - rect.top;

    // resize the height of glWin and reposition glDialog & status bar
    int glHeight = height - formHeight - statusHeight;
    ::SetWindowPos(glHandle, 0, 0, 0, width, glHeight, SWP_NOZORDER);
    ::SetWindowPos(formHandle, 0, 0, glHeight, width, formHeight, SWP_NOZORDER);
    ::InvalidateRect(formHandle, 0, TRUE);      // force to repaint
    ::SendMessage(statusHandle, WM_SIZE, 0, 0); // automatically resize width, so send 0s
    ::InvalidateRect(statusHandle, 0, FALSE);   // force to repaint

    // display OpenGL window dimension on the status bar
    std::wstringstream wss;
    wss << "Window Size (Client Area): " << width << " x " << height;
    ::SendMessage(statusHandle, SB_SETTEXT, 0, (LPARAM)wss.str().c_str());

    return 0;
}

//// handle WM_KEYDOWN
//int Win::ControllerMain::keyDown(int key, LPARAM lParam)
//{
//	switch (key)
//	{
//	case 'A':
//		for (auto i : model->shapes)
//			i->marked = true;
//		break;
//	}
//	return 0;
//}



///////////////////////////////////////////////////////////////////////////////
// enumerate all child windows
///////////////////////////////////////////////////////////////////////////////
bool CALLBACK enumerateChildren(HWND handle, LPARAM lParam)
{
    if(lParam == WM_CLOSE)
    {
        ::SendMessage(handle, WM_CLOSE, 0, 0);      // close child windows
    }

    return true;
}
