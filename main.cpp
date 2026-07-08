#include "Board.h"
#include "Timer.h"
#include "Viewer.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <functional>
using namespace std;

// Thread Variables
int					start		;
int					endConfig	;
vector<std::thread*>		threads		;
vector<Board*>		boards		;
std::thread*				viewThread	= nullptr;
int					numThreads	= 0;
int					runView		= -1;
Viewer*				viewer		;

// Main Thread Function
void runThread(int threadID, std::string dataset, std::string saveFile, std::string pathFile, bool loading)
{
	cout << "* Thread " << threadID + 1 << " active." << endl;

	Board* b;
	if (loading)
	{
		b = new Board(threadID+1, saveFile);
		boards.push_back(b);
		b->solveFromLoad();
	}
	else
	{
		b = new Board(threadID+1, dataset, saveFile, pathFile);
		boards.push_back(b);
		b->solve(start, endConfig);
	}
	cout << "* Thread " << threadID + 1 << " stopped." << endl;
	threads[threadID] = nullptr;
	boards[threadID] = nullptr;
	delete b;
}

// View Thread Function
void runViewer()
{
	if (!viewer)
	{
		// Hide Irrlicht junk
		FILE* redirectedStream = nullptr;
		freopen_s(&redirectedStream, "files/junk.txt", "w", stdout);
		viewer = new Viewer(boards[runView]);
		freopen_s(&redirectedStream, "CON", "w", stdout);
		cout << "+ Viewer opened" << endl;
//		system("del junk.txt");
	}
	while (runView != -1)
	{
		if (viewer) viewer->run();
	}
	if (viewer) delete viewer;
	viewer = nullptr;
	viewThread = nullptr;
}

// Main Function
int main(int argc, char** argv)
{ 
	cout << "\t\t\tETERNITY 2 SOLVER - MARK IV" << endl;
	cout << "\t\t\t---------------------------" << endl;

	std::string input;

	while (input != "quit" && input != "QUIT" && input != "exit" && input != "EXIT")
	{
		cout << ":-> ";
		cin >> input;

		if (input == "help" || input == "HELP" || input == "HELP!")
		{
			cout << "Welcome! These are the commands, listed here in helpful fashion for your\nconvenience:" << endl;
			cout << "HELP	- This command." << endl;
			cout << "CLS    - Clear Last Screen." << endl;
			cout << "LOAD	- Loads a thread with a previously saved state file." << endl;
			cout << "NOVIEW - Stops The Viewer (TM)." << endl;
			cout << "QSTART - QuickStart (TM). Starts a new thread quickly, using defaults." << endl;
			cout << "QUIT	- Exit the program, saving all active threads." << endl;
			cout << "RUN	- Runs a batch file, which will automatically start/load a number of threads." << endl;
			cout << "SILLY  - \"Does nothing.\"" << endl;
			cout << "START	- Starts a new thread; will ask for running parameters." << endl;
			cout << "STATUS - Lists all threads and their status." << endl;
			cout << "STOP	- Stops the given thread." << endl;
			cout << "TIMER  - Runs a time test using the online dataset 10 times." << endl;
			cout << "VIEW   - Views a thread with The Viewer (TM)." << endl;
		}
		else if (input == "SILLY" || input == "silly")
		{
			cout << "This does nothing, remember?" << endl;
		}
		else if (input == "START" || input == "start")
		{
			cout << "+ Start a new thread" << endl;
			std::string dataset;
			cout << "  Choose dataset -" << endl;
			cout << "  1. Eternity 2" << endl;
			cout << "  2. Clue 1" << endl;
			cout << "  3. Clue 2" << endl;
			cout << "  4. Online example" << endl;
			cout << "  Please enter the number of the dataset you wish to use, or type a new\n  filename:\n-> ";
			cin >> dataset;
			if (dataset == "1")			dataset = "datasets/Dataset_Eternity2.txt";
			else if (dataset == "2")	dataset = "datasets/Dataset_clue1.txt";
			else if (dataset == "3")	dataset = "datasets/Dataset_clue2.txt";
			else if (dataset == "4")	dataset = "datasets/Dataset_online.txt";
			cout << "  Please enter the corner config you wish to start with (0-23) :\n:-> ";
			int startConfig = 0;
			int endConfig = 23;
			cin >> startConfig;
			if (startConfig < 23)
			{
				cout << "  Please enter the corner config you with to end with (" << startConfig+1 << "-24) :\n-> ";
				cin >> endConfig;
			}
			if (startConfig < 0) startConfig = 0;
			if (startConfig > 24) startConfig = 24;
			if (startConfig == endConfig) endConfig = startConfig + 1;
			if (endConfig < startConfig || endConfig > 24) endConfig = 24;
			cout << "  Please enter the name for this thread (will be used as save filename):\n-> ";
			std::string saveFile;
			cin >> saveFile;

			cout << "  Do you wish to use a path? If so, type it in. You can also type" << endl;
			cout << "  VERTICAL or HORIZONTAL, or otherwise type \"N\" or \"n\" for no path.\n->" << endl;
			std::string pathFile;
			cin >> pathFile;

			cout << "  Please confirm these parameters:" << endl;
			cout << "  - Dataset: " << dataset << endl;
			cout << "  - Start:   " << startConfig << endl;
			cout << "  - End:     " << endConfig << endl;
			cout << "  - Name:    " << saveFile << endl;
			cout << "  - Path:    " << pathFile << endl;
			cout << "  Are these correct (Y/N)?:\n-> ";
			cin >> input;
			if (input == "Y" || input == "y")
			{
				// Start thread 
				cout << "+ Starting thread " << numThreads + 1 << endl;
			
				// Make function object
				start = startConfig;
				endConfig = endConfig;
				std::function<void (void)> runner = std::bind(&runThread, numThreads, dataset, saveFile, pathFile, false);

				// Activate it
				threads.push_back(new std::thread(runner)); 
				numThreads++;
				std::this_thread::sleep_for(std::chrono::milliseconds(250));
			}
		}
		else if (input == "QSTART" || input == "qstart")
		{
			cout << "+ QuickStart (TM)" << endl;
			cout << "  Using dataset:	Eternity 2" << endl;
			cout << "  Start:			0" << endl;
			cout << "  End:				24" << endl;
			cout << "  Name:			quickstart.txt" << endl;

			// Start thread 
			cout << "+ Starting thread " << numThreads + 1 << endl;
			
			// Make function object
			start = 0;
			endConfig = 24;
			std::function<void (void)> runner = std::bind(&runThread, numThreads, "datasets/Dataset_Eternity2.txt", "quickstart.txt", "", false);

			// Activate it
			threads.push_back(new std::thread(runner)); 
			numThreads++;
			std::this_thread::sleep_for(std::chrono::milliseconds(250));
		}
		else if (input == "LOAD" || input == "load")
		{
			cout << "+ Load a previous thread" << endl;
			std::string filename;
			cout << "  NB: All loaded/saved state files are stored in the \"files\" directory.\n";
			cout << "  Please enter the file name you wish to load:\n:-> ";
			cin >> filename;
			cout << "  Please confirm this parameter:" << endl;
			cout << "  - File name: " << filename << endl;
			cout << "  Is this correct (Y/N)?:\n-> ";
			cin >> input;
			if (input == "Y" || input == "y")
			{
				// Start thread 
				cout << "+ Loading thread " << numThreads + 1 << endl;
			
				// Make function object
				std::function<void (void)> runner = std::bind(&runThread, numThreads, "", filename, "", true);

				// Activate it
				threads.push_back(new std::thread(runner)); 
				numThreads++;
				std::this_thread::sleep_for(std::chrono::milliseconds(250));
			}
		}
		else if (input == "STOP" || input == "stop")
		{
			cout << "+ Stop a running thread" << endl;
			for (int i = 0; i < numThreads; i++)
			{
				if (threads[i])
				{
					cout << "  #" << i + 1 << " - " << boards[i]->getSaveName() << endl;
				}
			}
			cout << "  Choose thread to stop (thread will be saved):\n-> ";
			int thread;
			cin >> thread;
			if (thread >= 1 && thread <= numThreads)
			{
				Board* b = boards[thread-1];
				if (b) 
				{
					// Check if the viewer is still running
					if (runView == thread - 1)
					{
						// Stop viewer
						runView = -1;
						viewThread->join();
					}
					b->stop();
					std::this_thread::sleep_for(std::chrono::milliseconds(250));
				}
				else
				{
					cout << "  That thread has already finished." << endl;
				}
			}
			else
			{
				cout << "  That is not a valid thread." << endl;
			}
		}
		else if (input == "STATUS" || input == "status")
		{
			cout << "+ Thread status:" << endl;
			for (int i = 0; i < numThreads; i++)
			{
				if (threads[i])
				{
					cout << "  #" << i + 1 << " running - " << boards[i]->getSaveName() << "  Furthest: " << boards[i]->getFurthestPathID() << endl;
				}
				else
				{
					cout << "  #" << i + 1 << " stopped" << endl;
				}
			}
			if (numThreads == 0) cout << "  No threads" << endl;
		}
		else if (input == "VIEW" || input == "view")
		{
			cout << "+ View a running thread" << endl;
			for (int i = 0; i < numThreads; i++)
			{
				if (threads[i])
				{
					cout << "  #" << i + 1 << " - " << boards[i]->getSaveName() << endl;
				}
			}
			cout << "  Choose thread to view:\n-> ";
			int thread;
			cin >> thread;
			if (thread >= 1 && thread <= numThreads)
			{
				Board* b = boards[thread-1];
				if (b) 
				{
					if (runView == -1 && !viewThread)
					{
						runView = thread-1;
						viewThread = new std::thread(&runViewer);
					}
					else
					{
						runView = thread-1;
						viewer->setBoard(boards[runView]);
					}
					std::this_thread::sleep_for(std::chrono::milliseconds(250));
				}
				else
				{
					cout << "  That thread has already finished." << endl;
				}
			}
			else
			{
				cout << "  That is not a valid thread." << endl;
			}
		}
		else if (input == "NOVIEW" || input == "noview")
		{
			runView = -1;
		}
		else if (input == "TIMER" || input == "timer")
		{
			// Performance timer
			Board* b = new Board(0,"datasets/Dataset_online.txt","onlinetimer.txt","VERTICAL");
			b->setRunSilent(true);
			Timer t;
			for (int i = 0; i < 1000; i++)
			{
				b->solve(0,0);
			}
			cout << "Time test total solutions: " << b->numSolutions() << endl;
			cout << "Time elapsed: " << t.getTimeElapsed() << "s" << endl;
			delete b;
		}
		else if (input == "CLS" || input == "cls")
		{
			system("CLS");
		}
		else if (input == "RUN" || input == "run")
		{
			cout << "+ Run a batch file" << endl;
			cout << "  Please enter the batch file to load:" << endl;
			cout << "-> ";
			std::string filename;
			cin >> filename;
			ifstream fin(filename.c_str());
			if (fin)
			{
				cout << "  Running batch file: " << endl;
				std::string data;
				fin >> data;
				while (!fin.eof())
				{
					// LOAD command
					if (data == "load")
					{
						// get load params
						std::string fileToLoad;
						fin >> fileToLoad;

						cout << "  - Load " << fileToLoad << endl;

						// Make function object
						std::function<void (void)> runner = std::bind(&runThread, numThreads, "", fileToLoad, "", true);

						// Activate it
						threads.push_back(new std::thread(runner)); 
						numThreads++;
						std::this_thread::sleep_for(std::chrono::milliseconds(1000));
					}
					// START command
					else if (data == "start")
					{
						// get start params
						std::string dataset;
						fin >> dataset;
						int startCorner, endCorner;
						fin >> startCorner >> endCorner;

						// Check dataset
						if (dataset == "1")			dataset = "datasets/Dataset_Eternity2.txt";
						else if (dataset == "2")	dataset = "datasets/Dataset_clue1.txt";
						else if (dataset == "3")	dataset = "datasets/Dataset_clue2.txt";
						else if (dataset == "4")	dataset = "datasets/Dataset_online.txt";

						// set start params
						start = startCorner;
						endConfig = endCorner;

						// load save name
						std::string saveName;
						fin >> saveName;

						// load path name
						std::string path;
						fin >> path;

						cout << "  - Start " << dataset << " S:" << startCorner << " E:" << endCorner << " " << saveName << " Path:" << path << endl;

						// Make function object
						std::function<void (void)> runner = std::bind(&runThread, numThreads, dataset, saveName, path, false);

						// Activate it
						threads.push_back(new std::thread(runner)); 
						numThreads++;
						std::this_thread::sleep_for(std::chrono::milliseconds(1000));
					}
					// Comments
					else if (data == "/*")
					{
						while (data != "*/")
						{
							fin >> data;
						}
					}
					fin >> data;
				}
			}
		}
	}

	// STOP ALL RUNNING THREADS
	runView = -1;
	if (viewThread)
	{
		viewThread->join();
	}
	for (int i = 0; i < (int)threads.size(); i++)
	{
		if (threads[i])
		{
			boards[i]->stop();
		}
		if (threads[i])
		{
			threads[i]->join();
		}
	}

	system("pause");

	return 0;
}
