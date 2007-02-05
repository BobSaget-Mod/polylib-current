/*
 *   This file was automatically generated by version 1.7 of cextract.
 *   Manual editing not recommended.
 *
 *   Created: Tue Nov  2 18:34:50 1999
 */
#ifndef _Matop_h_
#define _Matop_h_
#if (defined(__STDC__) || defined(__cplusplus))

#if defined(__cplusplus)
extern "C" {
#endif

/* computes c = lcm(a,b) using Gcd(a,b,&c) */
extern void Lcm3(Value a, Value b, Value *c);
extern Matrix *AddANullColumn ( Matrix *M );
extern Matrix *AddANullRow ( Matrix *M );
extern void ExchangeColumns ( Matrix *M, int Column1, int Column2 );
extern void ExchangeRows ( Matrix *M, int Row1, int Row2 );
extern int findHermiteBasis ( Matrix *M, Matrix **Result );
extern Matrix *Identity ( unsigned size );
extern Bool isinHnf ( Matrix *A );
extern Bool isIntegral ( Matrix *A );
extern Value *Lcm (Value i, Value j);
extern Matrix * Matrix_Copy ( Matrix *Src );
extern void PutColumnFirst ( Matrix *X, int Columnnumber );
extern void PutColumnLast ( Matrix *X, int Columnnumber );
extern void PutRowFirst ( Matrix *X, int Rownumber );
extern void PutRowLast ( Matrix *X, int Rownumber );
extern Matrix *RemoveNColumns ( Matrix *M, int FirstColumnnumber, int NumColumns );
extern Matrix *RemoveColumn ( Matrix *M, int Columnnumber );
extern Matrix *RemoveRow ( Matrix *M, int Rownumber );
extern Matrix *Transpose ( Matrix *A );

#if defined(__cplusplus)
}
#endif

#else /* (defined(__STDC__) || defined(__cplusplus)) */

extern Matrix *AddANullColumn (/* Matrix *M */);
extern Matrix *AddANullRow (/* Matrix *M */);
extern void ExchangeColumns (/* Matrix *M, int Column1, int Column2 */);
extern void ExchangeRows (/* Matrix *M, int Row1, int Row2 */);
extern int findHermiteBasis (/* Matrix *M, Matrix **Result */);
extern Matrix *Identity (/* unsigned size */);
extern Bool isinHnf (/* Matrix *A */);
extern Bool isIntegral (/* Matrix *A */);
extern Value *Lcm (/* Value i, Value j */);
extern Matrix * Matrix_Copy (/* Matrix *Src */);
extern void PutColumnFirst (/* Matrix *X, int Columnnumber */);
extern void PutColumnLast (/* Matrix *X, int Columnnumber */);
extern void PutRowFirst (/* Matrix *X, int Rownumber */);
extern void PutRowLast (/* Matrix *X, int Rownumber */);
extern Matrix *RemoveNColumns (/* Matrix *M, int FirstColumnnumber, int NumColumns */)
extern Matrix *RemoveColumn (/* Matrix *M, int Columnnumber */);
extern Matrix *RemoveRow (/* Matrix *M, int Rownumber */);
extern Matrix *Transpose (/* Matrix *A */);

#endif /* (defined(__STDC__) || defined(__cplusplus)) */
#endif /* _Matop_h_ */
