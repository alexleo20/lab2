#include <stdio.h>
#include <ctime>
#include <cilk/cilk.h>
#include <cilk/reducer_opadd.h>
#include <chrono>
#include <iostream>

using namespace std::chrono;
using namespace std;

// ���������� ����� � �������� ���������� �������
const int MATRIX_SIZE = 1500;


/// ������� InitMatrix() ��������� ���������� � �������� 
/// ��������� ���������� ������� ���������� ����������
/// matrix - �������� ������� ����
void InitMatrix(double** matrix)
{
	for (int i = 0; i < MATRIX_SIZE; ++i)
	{
		matrix[i] = new double[MATRIX_SIZE + 1];
	}

	for (int i = 0; i < MATRIX_SIZE; ++i)
	{
		for (int j = 0; j <= MATRIX_SIZE; ++j)
		{
			matrix[i][j] = rand() % 2500 + 1;
		}
	}
}

/// ������� SerialGaussMethod() ������ ���� ������� ������ 
/// matrix - �������� ������� �������������� ���������, �������� � ����,
/// ��������� ������� ������� - �������� ������ ������ ���������
/// rows - ���������� ����� � �������� �������
/// result - ������ ������� ����
void SerialGaussMethod(double **matrix, const int rows, double* result)
{
	int k;
	double koef;

	// ������ ��� ������ ������
	//��������� ������� ������� ����
	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	for (k = 0; k < rows; ++k)
	{

		for (int i = k + 1; i < rows; ++i)
		{
			koef = -matrix[i][k] / matrix[k][k];
			for (int j = k; j <= rows; ++j)
			{
				matrix[i][j] += koef * matrix[k][j];
			}
		}
	}

	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	duration<double> duration = (t2 - t1);
	cout << "Forward elimination duration for 1500-lines matrix is " << duration.count() << " seconds" << endl;

	// �������� ��� ������ ������
	result[rows - 1] = matrix[rows - 1][rows] / matrix[rows - 1][rows - 1];

	for (k = rows - 2; k >= 0; --k)
	{
		result[k] = matrix[k][rows];

		for (int j = k + 1; j < rows; ++j)
		{
			result[k] -= matrix[k][j] * result[j];
		}

		result[k] /= matrix[k][k];
	}
}


int main()
{
	srand((unsigned)time(0));

	int i;

	// ���-�� ����� � �������, ���������� � �������� �������
	//const int test_matrix_lines = 4;

	double **test_matrix = new double*[MATRIX_SIZE];

	// ���� �� �������
	for (i = 0; i < MATRIX_SIZE; ++i)
	{
		// (test_matrix_lines + 1)- ���������� �������� � �������� �������,
		// ��������� ������� ������� ������� ��� ������ ����� ���������, �������� � ����
		test_matrix[i] = new double[MATRIX_SIZE + 1];
	}

	// ������ ������� ����
	double *result = new double[MATRIX_SIZE];

	//��������� ������� ���������� �������
	InitMatrix(test_matrix);

	SerialGaussMethod(test_matrix, MATRIX_SIZE, result);



	for (i = 0; i < MATRIX_SIZE; ++i)
	{
		delete[]test_matrix[i];
	}
	delete[] result;

	return 0;
}