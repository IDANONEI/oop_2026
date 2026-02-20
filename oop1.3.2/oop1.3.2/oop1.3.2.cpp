#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <iomanip>
#include <array>

using Matrix3x3 = std::array<std::array<double, 3>, 3>;

constexpr std::string_view HELP_FLAG = "-h";


enum class ReadError
{
    Ok,
    Format,
    Value
};

void PrintHelp(std::ostream& out)
{
    out << "Usage:\n"
        << "  invert <input_file>\n"
        << "  invert            (read matrix from stdin)\n"
        << "  invert -h         (help)\n";
}

bool IsHelpFlag(std::string_view param)
{
    return param == HELP_FLAG;
}

ReadError TryGet3Matrix(Matrix3x3& matrix, std::istream& in)
{
    std::string line;

    for (int i = 0; i < 3; ++i)
    {
        if (!std::getline(in, line))
            return ReadError::Format;

        std::istringstream iss(line);


        for (int j = 0; j < 3; ++j)
        {
            if (!(iss >> matrix[i][j]))
                return ReadError::Value;

            if (j < 2)
            {
                char sep;
                if (!iss.get(sep) || sep != '\t')
                    return ReadError::Format;
            }
        }

        iss >> std::ws;
        if (!iss.eof())
            return ReadError::Format;

    }

    return ReadError::Ok;
}

bool TryReadMatrix(Matrix3x3& matrix, const char* filename = nullptr)
{
    std::istream* in = &std::cin;
    std::ifstream file;

    if (filename)
    {
        file.open(filename);
        if (!file)
        {
            std::cout << "Invalid matrix format";
            return false;
        }
        in = &file;
    }

    ReadError err = TryGet3Matrix(matrix, *in);

    if (err == ReadError::Format)
        std::cout << "Invalid matrix format";
    else if (err == ReadError::Value)
        std::cout << "Invalid matrix";

    return err == ReadError::Ok;
}

bool Inverse3x3(Matrix3x3& matrix, Matrix3x3& inverseM)
{
    const double a = matrix[0][0], b = matrix[0][1], c = matrix[0][2];
    const double d = matrix[1][0], e = matrix[1][1], f = matrix[1][2];
    const double g = matrix[2][0], h = matrix[2][1], i = matrix[2][2];

    double det = a * (e * i - f * h) - b * (d * i - f * g) + c * (d * h - e * g);

    if (det == 0)
        return false;

    Matrix3x3 C;
    C[0][0] = (e * i - f * h);
    C[0][1] = -(d * i - f * g);
    C[0][2] = (d * h - e * g);

    C[1][0] = -(b * i - c * h);
    C[1][1] = (a * i - c * g);
    C[1][2] = -(a * h - b * g);

    C[2][0] = (b * f - c * e);
    C[2][1] = -(a * f - c * d);
    C[2][2] = (a * e - b * d);

    double invDet = 1.0 / det;
    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            inverseM[row][col] = C[col][row] * invDet;
        }
    }

    return true;
}

void PrintMatrix3x3(Matrix3x3 &matrix)
{
    std::cout << std::fixed << std::setprecision(3);
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            std::cout << matrix[i][j];
            if (j < 2) std::cout << '\t';
        }
        if (i < 2) std::cout << std::endl;
    }
}

int main(int argc, char* argv[])
{
    Matrix3x3 matrix;

    if (argc == 2)
    {
        if (IsHelpFlag(argv[1]))
        {
            PrintHelp(std::cout);
            return EXIT_SUCCESS;
        }

        if (!TryReadMatrix(matrix, argv[1]))
            return EXIT_FAILURE;
    }

    if (argc > 2)
    {
        std::cout << "Invalid matrix format";
        return EXIT_FAILURE;
    }

    if (argc == 1)
    {
        if (!TryReadMatrix(matrix))
            return EXIT_FAILURE;
    }

    Matrix3x3 inv;
    if (!Inverse3x3(matrix, inv))
    {
        std::cout << "Non-invertible";
        return EXIT_SUCCESS;
    }

    PrintMatrix3x3(inv);
    return EXIT_SUCCESS;
}
