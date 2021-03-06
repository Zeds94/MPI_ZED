// ConsoleApplication1.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

// mpiTest.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
//#include <stdio.h>
#include "pch.h"
#include <mpi.h>
#include <math.h>
#include <iostream>

using namespace std;

int main(int argc, char **argv)
{
	int n, rank, numProcs, left, right;
	double realPi = 3.141592653589793238462643;
	double myPi, delta, f, x, sum, error, f_accum;

	//Initialize MPI
	MPI_Init(&argc, &argv);

	MPI_Request request, request2;
	MPI_Status status;

	//Get my rank
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	// Get my number of processors
	MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
	cout << "I am process " << rank << " of " << numProcs << "\n";

	n = 1000000;
	delta = 1.0 / double(n);
	f = 0.0;

	//Calculate a part of pi by each processor
	for (int i = rank + 1; i <= n; i += numProcs) {
		x = delta * (double(i) - 0.5);
		f += 4.0 / (1.0 + x * x);
	}

	MPI_Isend(&f, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &request);

	//Calculate global pi
	for (int i = 1; i < numProcs; ++i) {
		MPI_Irecv(&f_accum, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &request2);
		f += f_accum;
		cout << "Acumulator: " << f_accum << "of the " << i  << "processor" << endl;
		MPI_Wait(&request, &status);
		MPI_Wait(&request2, &status);
	}
	//calculate Pi, compare to the Pi to 3.1415926
	myPi = f / n;
	cout.precision(14);
	cout << "Process #" << rank << endl;
	cout << "pi is approximately: " << myPi << endl;
	cout << "error is :" << fabs(myPi - realPi) << endl;

	MPI_Finalize();
	return 0;

}

// Exécuter le programme : Ctrl+F5 ou menu Déboguer > Exécuter sans débogage
// Déboguer le programme : F5 ou menu Déboguer > Démarrer le débogage

// Conseils pour bien démarrer : 
//   1. Utilisez la fenêtre Explorateur de solutions pour ajouter des fichiers et les gérer.
//   2. Utilisez la fenêtre Team Explorer pour vous connecter au contrôle de code source.
//   3. Utilisez la fenêtre Sortie pour voir la sortie de la génération et d'autres messages.
//   4. Utilisez la fenêtre Liste d'erreurs pour voir les erreurs.
//   5. Accédez à Projet > Ajouter un nouvel élément pour créer des fichiers de code, ou à Projet > Ajouter un élément existant pour ajouter des fichiers de code existants au projet.
//   6. Pour rouvrir ce projet plus tard, accédez à Fichier > Ouvrir > Projet et sélectionnez le fichier .sln.
