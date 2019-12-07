#pragma once
#include <complex>
#include <cstring>
#include <sstream>
#include <stdexcept>
#include <type_traits>
#include <iostream>

namespace ELCT350
{
  /**
   * @class Matrix
   * @brief Matrix object with matrix arithmetic operators
   * @details
   * This class provides an implementation of a matrix that can both hold numerical values and perform matrix
   * operations on itself or in combination with other matrix instances
   **/
  template<typename Numeric>
  class Matrix final
  {
  public:
    #pragma region Constructors and Destructors
    /**
     * Constructor for Matrix object
     * @param[in] rows The number of rows in the matrix
     * @param[in] columns The number of columns in the matrix
     * @param[in] initializationValue
     * Optional argument with the initialization value for all the cells in the matrix. If not called, then the default
     * for the datatype will be used.
     **/
    Matrix(size_t rows, size_t columns, const Numeric& initializationValue = Numeric{})
      : _rows(rows), _columns(columns), _data(new Numeric*[rows])
    {
      //If the type is not an arithmetic type (for instance, a string), we don't allow it to build
      static_assert(std::is_arithmetic_v<Numeric> || std::is_same_v< Numeric, std::complex<double> >, "Matrix must be of a numeric type");

      //We initalize the _data array with the columns for every row, and the value for each cell
      for (size_t row = 0; row < _rows; ++row)
      {
        _data[row] = new Numeric[columns]; //allocate memory for the columns
        for (size_t column = 0; column < _columns; ++column)
        {
          _data[row][column] = initializationValue; //set the initialization value
        }
      }
    }

    /**
     * Copy constructor for Matrix
     * @param[in] original The matrix we will copy
     */
    Matrix(const Matrix<Numeric>& original)
      : _rows(original._rows), _columns(original._columns), _data(new Numeric*[_rows])
    {
      //Allocate memory and initialize cells with values from original matrix
      for (size_t row = 0; row < _rows; ++row)
      {
        _data[row] = new Numeric[_columns];
        for (size_t column = 0; column < _columns; ++column)
        {
          _data[row][column] = original._data[row][column];
        }
      }
    }

    /**
     * Move constructor for Matrix
     * @param old The matrix we're moving to a new object (and will be invalidated after this constructor returns)
     **/
    Matrix(Matrix<Numeric>&& old) noexcept
      : _rows(old._rows), _columns(old._columns), _data(old._data)
    {
      //We have to remove the old matrix access to the data pointer, so we don't end up deleting the same data twice
      old._rows = 0;
      old._columns = 0;
      old._data = nullptr;
    }

    /**
     * Destructor for Matrix. Called when the instance goes out of scope, or it is explicitly deleted
     **/
    ~Matrix()
    {
      deleteData();
    }
    #pragma endregion

    #pragma region Modifiers
    void zeroMatrix()
    {
      for(size_t row = 0; row < _rows; ++row)
      {
        memset(_data[row], 0, _columns * sizeof(Numeric));
      }
    }
    #pragma endregion

    #pragma region Observers
    /**
     * @return the number of rows in the matrix
     **/
    size_t getRows() const
    {
      return _rows;
    }

    /**
     * @return the number of columns in the matrix
     **/
    size_t getColumns() const
    {
      return _columns;
    }

    /**
     * Computes the inverse of the matrix
     * @return the inverse of this matrix instance
     **/
    Matrix<Numeric> getInverse() const
    {
      //We can only compute the inverse for a square matrix, so we throw an exception if it's not square
      if (_rows != _columns)
        throw std::domain_error("Matrix must be square to have an inverse");

      //Get LU Decomposition
      Matrix<Numeric> L(_rows, _columns);
      Matrix<Numeric> U(_rows, _columns);
      decomposeLU(L, U);

      //Compute the inverse of L and U
      Matrix<Numeric> inverseL(_rows, _columns);
      Matrix<Numeric> inverseU(_rows, _columns);
      
      //We can use the fact that matrices are triangular to compute their inverse in a fairly straight-forward way.
      //Ordinarily, for MNA, we would never actually compute the inverse. A minor modification of the below algorithm
      //can be used to solve directly for x in Ax=b (LUx=b after decomposition). In fact, that's kind of what we're doing
      //here, where b is actually I, the identity matrix (matrix with 1s in the diagonal cells, but 0s elsewhere)
      for (size_t row = 0; row < _rows; ++row)
      {
        inverseL(row, row) = static_cast<Numeric>(1) / L(row, row);
        inverseU(row, row) = static_cast<Numeric>(1) / U(row, row);
        for (size_t l = 0; l < row; ++l)
        {
          for (size_t j = 0; j < row; ++j)
          {
            inverseL(row, l) -= (L(row, j) * inverseL(j, l)) / L(row, row);
            inverseU(l, row) -= inverseU(l, j) * U(j, row) / U(row, row);
          }
        } 
      }

      //return U^(-1)*L^(-1), which is the inverse of the matrix
      return inverseU * inverseL;
    }

    /**
     * Computes a string representation of this matrix
     * @return string represenation of this matrix
     **/
    std::string toString() const
    {
      std::stringstream stream;
      for (size_t row = 0; row < _rows; ++row)
      {
        stream << "[ ";
        for (size_t column = 0; column < _columns; ++column)
        {
          stream << _data[row][column] << " ";
        }
        stream << "]\n";
      }

      return stream.str();
    }
    #pragma endregion

    #pragma region Operators
    /**
     * Accesses a cell in the matrix
     * @param[in] row Row index
     * @param[in] column Column index
     * @return a reference to the cell.
     **/
    Numeric& operator()(size_t row, size_t column)
    {
      checkBounds(row, column);
      return _data[row][column];
    }

    /**
     * Accesses a cell in the matrix, but does not allow modification of that cell
     * @param[in] row Row index
     * @param[in] column Column index
     * @return a const reference to the cell.
     **/
    const Numeric& operator()(size_t row, size_t column) const
    {
      checkBounds(row, column);
      return _data[row][column];
    }

    /**
     * Computes the sum of this matrix with another
     * @param[in] addend The addend matrix
     * @return the sum of this matrix with the addend
     **/
    Matrix<Numeric> operator+(const Matrix<Numeric>& addend) const
    {
      if ((_rows != addend.getRows()) || (_columns != addend.getColumns()))
        throw std::domain_error("Matrices must be the same size to be added");

      Matrix<Numeric> result(_rows, _columns);

      #pragma region Add Addition logic here!
      
      #pragma endregion

      return result;
    }

    /**
     * Computes the difference of this matrix and another
     * @param[in] subtrahend The subtrahend matrix
     * @return the difference of this matrix and another
     **/
    Matrix<Numeric> operator-(const Matrix<Numeric>& subtrahend) const
    {
      if ((_rows != subtrahend.getRows()) || (_columns != subtrahend.getColumns()))
        throw std::domain_error("Matrices must be the same size to be subtracted");

      Matrix<Numeric> result(_rows, _columns);

      #pragma region Add Subtraction logic here!
     
      #pragma endregion

      return result;
    }

    /**
     * Computes the product of this matrix and another
     * @param[in] multiplicand The multiplicand matrix
     * @return The product between this matrix and the multiplicand
     **/
    Matrix<Numeric> operator*(const Matrix<Numeric>& multiplicand) const
    {      
      if (_columns != multiplicand.getRows())
        throw std::domain_error("The number of columns on the multiplicand must match the number of rows in the multiplier");

      auto columns = multiplicand.getColumns();
      Matrix<Numeric> product(_rows, columns);

      #pragma region Add Multiplication logic here!
      
      #pragma endregion

      return product;
    }

    Matrix<Numeric>& operator=(const Matrix<Numeric>& rhs)
    {
      if (_data)
      {
        if (_rows != rhs._rows || _columns != rhs._columns)
        {
          for (size_t row = 0; row < _rows; ++row)
            delete[] _data[row];
          delete[] _data;

          _rows = rhs._rows;
          _columns = rhs._columns;
          _data = new Numeric*[_rows];
          for (size_t row = 0; row < _rows; ++row)
            _data[row] = new Numeric[_columns];
        }
      }

      for (size_t row = 0; row < _rows; ++row)
        memcpy(_data[row], rhs._data[row], _columns * sizeof(Numeric));

      return *this;
    }

    Matrix<Numeric>& operator=(Matrix<Numeric>&& rhs) noexcept
    {
      _rows = rhs._rows;
      _columns = rhs._columns;
      _data = rhs._data;

      rhs._rows = 0;
      rhs._columns = 0;
      rhs._data = nullptr;

      return *this;
    }
    #pragma endregion
  private:
    #pragma region Variables
    size_t _rows; //number of rows
    size_t _columns; //number of columns
    Numeric** _data; //array structure holding the values
    #pragma endregion

    #pragma region Functions
    /**
     * Function that checks to see if indices are within bounds and throws an exception if they're not
     **/
    inline void checkBounds(size_t row, size_t column) const
    {
      if (row >= _rows || column >= _columns)
        throw std::out_of_range("Index out of bounds for matrix");
    }

    void deleteData()
    {
      if (_data) //if our _data pointer is null, we don't do anything
      {
        //Delete all the columns associated with each row
        for (size_t row = 0; row < _rows; ++row)
        {
          delete[] _data[row];
        }

        //Delete the rows
        delete[] _data;
      }
    }

    /**
     * Computes the LU decomposition of this matrix
     * param[out] L The lower triangular matrix factor
     * param[out] U The upper trianfular matrix factor
     **/
    void decomposeLU(Matrix<Numeric>& L, Matrix<Numeric>& U) const
    {
      //We assume that L and U are the same size of this matrix and that the matrix is square.
      //We're not checking because this is a private function, so we can control how it's called
      //(and ensure it's checked before this function is called)

      //Compute the values of U
      for (size_t i = 0; i < _rows; ++i)
      {
        for (size_t j = i; j < _rows; ++j)
        {
          Numeric sum = Numeric{};
          for (size_t k = 0; k < i; ++k)
          {
            sum += L(i, k) * U(k, j);
          }
          U(i, j) = _data[i][j] - sum;
        }

        //Compute the values of L
        for (size_t j = i; j < _rows; ++j)
        {
          if (i == j)
            L(i, j) = 1;
          else
          {
            Numeric sum = Numeric{};
            for (size_t k = 0; k < i; ++k)
            {
              sum += L(j, k) * U(k, i);
            }

            L(j, i) = (_data[j][i] - sum) / U(i, i);
          }
        }
      }
    }
    #pragma endregion
  };
}
