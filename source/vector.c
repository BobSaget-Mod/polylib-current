/* vector.c
          COPYRIGHT
          Both this software and its documentation are

              Copyrighted 1993 by IRISA /Universite de Rennes I - France,
              Copyright 1995,1996 by BYU, Provo, Utah
                         all rights reserved.

          Permission is granted to copy, use, and distribute
          for any commercial or noncommercial purpose under the terms
          of the GNU General Public license, version 2, June 1991
          (see file : LICENSING).
*/

#include <stdio.h>
#include <stdlib.h>
#include <polylib/polylib.h>

#ifdef MAC_OS
  #define abs __abs
#endif

/* 
 * Compute n! 
 */
Value *Factorial(int n) {
  
  int i;
  Value *fact, tmp;
  
  fact = (Value *)malloc(sizeof(Value));
  value_init(*fact); value_init(tmp);
  
  value_set_si(*fact,1);
  for (i=1;i<=n;i++) {
    value_set_si(tmp,i);
    value_multiply(*fact,*fact,tmp);
  }
  value_clear(tmp);
  return fact;
} /* Factorial */

/* 
 * Compute n!/(p!(n-p)!) 
 */
Value *Binomial(int n, int p) {
  
  int i;
  Value *result,tmp;
  
  result = (Value *)malloc(sizeof(Value));
  value_init(*result); value_init(tmp);
  
  if (n-p<p)
    p=n-p;
  if (p!=0) {
    value_set_si(*result,(n-p+1));
    for (i=n-p+2;i<=n;i++) {
      value_set_si(tmp,i);    
      value_multiply(*result,*result,tmp);
    }  
    value_division(*result,*result,*Factorial(p));
  }
  else 
    value_set_si(*result,1);
  value_clear(tmp);
  return result;
} /* Binomial */
  
/*
 * Return the number of ways to choose 'b' items from 'a' items, that is, 
 * return a!/(b!(a-b)!)
 */
Value *CNP(int a,int b) {
  
  int i;
  Value *result,tmp;
  
  result = (Value *)malloc(sizeof(Value));
  value_init(*result); value_init(tmp);

  value_set_si(*result,1);
  
  /* If number of items is less than the number to be choosen, return 1 */
  if(a <= b) {
    value_clear(tmp);
    return result;
  }  
  for(i=a;i>b;--i) {
    value_set_si(tmp,i);
    value_multiply(*result,*result,tmp);
  }  
  for(i=1;i<=(a-b);++i) {
    value_set_si(tmp,i);
    value_division(*result,*result,tmp);
  }
  value_clear(tmp);
  return result;
} /* CNP */

/* 
 * Compute GCD of 'a' and 'b' 
 */
Value *Gcd(Value a,Value b) {

  Value *result;
  Value acopy, bcopy;

  value_init(acopy);
  value_init(bcopy);
  result = (Value *)malloc(sizeof(Value));  
  if (!result) {
    fprintf(stderr,"Not enough memory\n");
    exit(0);
  }  
  value_init(*result);   
  value_assign(acopy,a);
  value_assign(bcopy,b);
  while(value_notzero_p(acopy)) { 
    value_modulus(*result,bcopy,acopy);      
    value_assign(bcopy,acopy);                     
    value_assign(acopy,*result);                   
  }
  value_absolute(*result,bcopy);
  value_clear(acopy);
  value_clear(bcopy);
  return(result);                           
} /* Gcd */

/* 
 * Return the smallest component index in 'p' whose value is non-zero 
 */
int First_Non_Zero(Value *p,unsigned length) {
  
  Value *cp;
  int i;
  
  cp = p;
  for (i=0;i<length;i++) {
    if (value_notzero_p(*cp))
      break;
    cp++;
  }
  return((i==length) ? -1 : i );
} /* First_Non_Zero */

/* 
 * Allocate memory space for Vector 
 */
Vector *Vector_Alloc(unsigned length) {

  int i;
  Vector *vector;
  
  vector = (Vector *)malloc(sizeof(Vector));
  if (!vector) {
    errormsg1("Vector_Alloc", "outofmem", "out of memory space");
    return 0;
  }
  vector->Size=length;
  vector->p=(Value *)malloc(length * sizeof(Value));
  if (!vector->p) {
    errormsg1("Vector_Alloc", "outofmem", "out of memory space");
    free(vector);
    return 0;
  }
  for(i=0;i<length;i++)
    value_init(vector->p[i]);
  return vector;
} /* Vector_Alloc */

/* 
 * Free the memory space occupied by Vector 
 */
void Vector_Free(Vector *vector) {
  
  int i;

  if (!vector) return;
  for(i=0;i<vector->Size;i++) 
    value_clear(vector->p[i]);
  free(vector->p);
  free(vector);
} /* Vector_Free */

/* 
 * Print the contents of a Vector 
 */
void Vector_Print(FILE *Dst,char *Format,Vector *vector) {
  
  int i;
  Value *p;
  unsigned length;
  
  fprintf(Dst, "%d\n", length=vector->Size);
  p = vector->p;
  for (i=0;i<length;i++) {
    if (Format) {
      value_print(Dst,Format,*p++);
    }  
    else {	
      value_print(Dst,P_VALUE_FMT,*p++);
    }  
  }
  fprintf(Dst, "\n");
} /* Vector_Print */

/* 
 * Read the contents of a Vector 
 */
Vector *Vector_Read() {
  
  Vector *vector;
  unsigned length;
  int i;
  char str[1024];
  Value *p;
  
  scanf("%d", &length);
  vector = Vector_Alloc(length);
  if (!vector) {
    errormsg1("Vector_Read", "outofmem", "out of memory space");
    return 0;
  }
  p = vector->p;
  for (i=0;i<length;i++) {
    scanf("%s",str);
    value_read(*(p++),str);
  }  
  return vector;
} /* Vector_Read */

/* 
 * Assign 'n' to each component of Vector 'p' 
 */
void Vector_Set(Value *p,int n,unsigned length) {
  
  Value *cp;
  int i;
  
  cp = p; 
  for (i=0;i<length;i++) {
    value_set_si(*cp,n);
    cp++;
  }
  return;
} /* Vector_Set */

/*
 * Exchange the components of the vectors 'p1' and 'p2' of length 'length'
 */
void Vector_Exchange(Value *p1, Value *p2, unsigned length) {

  int i;
  
  for(i=0;i<length;i++) {
    value_swap(p1[i],p2[i]);
  }  
  return;
}

/*
 * Copy Vector 'p1' to Vector 'p2' 
 */
void Vector_Copy(Value *p1,Value *p2,unsigned length) {

  int i;
  Value *cp1, *cp2;

  cp1 = p1;
  cp2 = p2;
  
  for(i=0;i<length;i++) 
    value_assign(*cp2++,*cp1++);
  
  return;
}
  
/* 
 * Add two vectors 'p1' and 'p2' and store the result in 'p3' 
 */
void Vector_Add(Value *p1,Value *p2,Value *p3,unsigned length) {

  Value *cp1, *cp2, *cp3;
  int i;
  
  cp1=p1;
  cp2=p2;
  cp3=p3;
  for (i=0;i<length;i++) {
    
    /* *cp3++ = *cp1++ + *cp2++ */
    value_addto(*cp3,*cp1,*cp2);
    cp1++; cp2++; cp3++;
  }
} /* Vector_Add */

/* 
 * Subtract two vectors 'p1' and 'p2' and store the result in 'p3' 
 */
void Vector_Sub(Value *p1,Value *p2,Value *p3,unsigned length) {

  Value *cp1, *cp2, *cp3;	
  int i;
  
  cp1=p1;
  cp2=p2;
  cp3=p3;
  for (i=0;i<length;i++) {
    
    /* *cp3++= *cp1++ - *cp2++ */
    value_substract(*cp3,*cp1,*cp2);
    cp1++; cp2++; cp3++;
  }
} /* Vector_Sub */

/* 
 * Compute Bitwise OR of Vectors 'p1' and 'p2' and store in 'p3' 
 */
void Vector_Or(Value *p1,Value *p2,Value *p3,unsigned length) {

  Value *cp1, *cp2, *cp3;
  int i;
  
  cp1=p1;
  cp2=p2;
  cp3=p3;
  for (i=0;i<length;i++) {
    
    /* *cp3++=*cp1++ | *cp2++ */
    value_orto(*cp3,*cp1,*cp2);
    cp1++; cp2++; cp3++;
  }
} /* Vector_Or */

/* 
 * Scale Vector 'p1' lambda times and store in 'p2' 
 */
void Vector_Scale(Value *p1,Value *p2,Value lambda,unsigned length) {
  
  Value *cp1, *cp2;
  int i;
  
  cp1=p1;
  cp2=p2;
  for (i=0;i<length;i++) {
    
    /* *cp2++=*cp1++ * lambda */
    value_multiply(*cp2,*cp1,lambda);
    cp1++; cp2++;
  }
} /* Vector_Scale */

/* 
 * Antiscale Vector 'p1' by lambda and store in 'p2' 
 */
void Vector_AntiScale(Value *p1,Value *p2,Value lambda,unsigned length) {
  
  Value *cp1, *cp2;
  int i;
  
  cp1=p1;
  cp2=p2;
  for (i=0;i<length;i++) {
    
    /* *cp2++=*cp1++ / lambda; */
    value_division(*cp2,*cp1,lambda);
    cp1++; cp2++;
  }
} /* Vector_AntiScale */

/* 
 * Return the inner product of the two Vectors 'p1' and 'p2' 
 */
Value *Inner_Product(Value *p1,Value *p2,unsigned length) {
  
  Value *ip,tmp;
  int i;

  ip = (Value *)malloc(sizeof(Value));
  value_init(*ip); value_init(tmp);

  //  if(length==0)
  //  return(VALUE_MAX);  
  
  value_multiply(*ip,*p1,*p2);
  p1++; p2++;
  for(i=1;i<length;i++) {
    value_multiply(tmp,*p1,*p2);
    value_addto(*ip,*ip,tmp);
    p1++; p2++;
  }
  value_clear(tmp);
  return ip;
} /* Inner_Product */

/* 
 * Return the maximum of the components of 'p' 
 */
Value *Vector_Max(Value *p,unsigned length) {
  
  Value *cp,*max;
  int i;

  max = (Value *)malloc(sizeof(Value));
  value_init(*max); 

  cp=p;
  value_assign(*max,*cp);
  cp++;
  for (i=1;i<length;i++) {
    value_maximum(*max,*max,*cp);
    cp++;
  }
  return max;
} /* Vector_Max */

/* 
 * Return the minimum of the components of Vector 'p' 
 */
Value *Vector_Min(Value *p,unsigned length) {
  
  Value *cp,*min;
  int i;

  min = (Value *)malloc(sizeof(Value));
  value_init(*min);

  //  if(length==0)
  //  return(VALUE_MAX);
  cp=p;
  value_assign(*min,*cp);
  cp++;
  for (i=1;i<length;i++) {
    value_minimum(*min,*min,*cp);
    cp++;
  }
  return min;
} /* Vector_Min */

/* 
 * Given Vectors 'p1' and 'p2', return Vector 'p3' = lambda * p1 + mu * p2. 
 */
void  Vector_Combine(Value *p1,Value *p2, Value *p3,Value lambda,Value  mu,unsigned length) {
  
  Value *cp1, *cp2, *cp3;
  Value tmp1, tmp2;
  int i;
  
  value_init(tmp1); value_init(tmp2);
  cp1=p1;
  cp2=p2;
  cp3=p3;
  
  for (i=0;i<length;i++) {
    
    /* tmp1 = lambda * *cp1 */
    value_multiply(tmp1,lambda,*cp1);
    
    /* tmp2 = mu * *cp2 */
    value_multiply(tmp2,mu,*cp2);
    
    /* *cp3 = tmp1 + tmp2 */
    value_addto(*cp3,tmp1,tmp2);
    cp1++; cp2++; cp3++;
  }
  value_clear(tmp1);
  value_clear(tmp2);
  return;
} /* Vector_Combine */

/* 
 * Return 1 if 'Vec1' equals 'Vec2', otherwise return 0 
 */
int Vector_Equal(Value *Vec1,Value *Vec2,unsigned n) {
  
  int i;
  Value *p1, *p2;
  
  p1=Vec1;
  p2=Vec2;
  for(i=0;i<n;i++) {
  
    /* if (*p1++!=*p2++) break; */
    if (value_ne(*p1,*p2))
      break;
    p1++; p2++;
  }
  return (i==n);
} /* Vector_Equal */

/* 
 * Return the component of 'p' with minimum non-zero absolute value. 'index'
 * points to the component index that has the minimum value. If no such value
 * and index is found, Value 1 is returned.
 */
Value *Vector_Min_Not_Zero(Value *p,unsigned length,int *index) {
  
  Value *cp,*min, aux;
  int i,j;
  
  min = (Value *)malloc(sizeof(Value));
  value_init(*min); value_init(aux);
  
  cp=p;
  for(i=0;i<length;i++) {
    if (value_notzero_p(*cp)) {
      value_absolute(*min,*cp);
      *index = i;
      break;
    }
    ++cp;
  }
  if (i == length) {
    value_set_si(*min,1);
    value_clear(aux);
    cp = NULL;
    return min;
  }
  ++cp;
  for(j=i+1;j<length;j++) {
    value_absolute(aux,*cp);
    if (value_lt(aux,*min) && value_notzero_p(aux)) {
      value_assign(*min,aux);
      *index = j;
    }  
    cp++;
  }
  value_clear(aux);
  cp = NULL;
  return min;
} /* Vector_Min_Not_Zero */

/* 
 * Return the GCD of components of Vector 'p' 
 */
Value *Vector_Gcd(Value *p,unsigned length) {
  
  Value *q,*cq, *cp, *min;
  int i, Not_Zero, Index_Min=0;
  
  min = (Value *)malloc(sizeof(Value));
  q  = (Value *)malloc(length*sizeof(Value));

  /* Initialize all the 'Value' variables */
  for(i=0;i<length;i++)
    value_init(q[i]);
  value_init(*min); 
  
  /* 'cp' points to vector 'p' and cq points to vector 'q' that holds the */
  /* absolute value of elements of vector 'p'.                            */
  cp=p;
  for (cq = q,i=0;i<length;i++) {
    value_absolute(*cq,*cp);    
    cq++;
    cp++;
  }
  do {   
    min = Vector_Min_Not_Zero(q,length,&Index_Min);     
    
    /* if (*min != 1) */
    if (value_notone_p(*min)) {
      
      cq=q;
      Not_Zero=0;
      for (i=0;i<length;i++,cq++)
        if (i!=Index_Min) {
          
	  /* Not_Zero |= (*cq %= *min) */
	  value_modulus(*cq,*cq,*min);
          Not_Zero |= value_notzero_p(*cq);
        }
    } 
    else 
      break;
  } while (Not_Zero);
  
  /* Clear all the 'Value' variables */
  for(i=0;i<length;i++)
    value_clear(q[i]);
  free(q);
  return min;
} /* Vector_Gcd */

/* 
 * Given vectors 'p1', 'p2', and a pointer to a function returning 'Value type,
 * compute p3[i] = f(p1[i],p2[i]).  
 */ 
void Vector_Map(Value *p1,Value *p2,Value *p3,unsigned length,Value *(*f)()) {
  
  Value *cp1, *cp2, *cp3;
  int i;
  
  cp1=p1;
  cp2=p2;
  cp3=p3;
  for(i=0;i<length;i++) {
    value_assign(*cp3,*(*f)(*cp1, *cp2));
    cp1++; cp2++; cp3++;
  }
  return;
} /* Vector_Map */

/* 
 * Reduce a vector by dividing it by GCD. There is no restriction on 
 * components of Vector 'p'. Making the last element positive is *not* OK
 * for equalities. 
 */
void Vector_Normalize(Value *p,unsigned length) {
  
  Value *cp, *gcd,tmp;
  int i;
  
  value_init(tmp);

  gcd = Vector_Gcd(p,length);
  value_set_si(tmp,1);
  
  if (value_gt(*gcd,tmp)) {
    cp = p;    
    for (i=0; i<length; i++) { 
      
      /* *cp /= gcd */
      value_division(*cp,*cp,*gcd);
      cp++;
    }
  }
  value_clear(tmp);
  value_clear(*gcd);
  free(gcd);
  return;
} /* Vector_Normalize */

/* 
 * Reduce a vector by dividing it by GCD and making sure its pos-th 
 * element is positive.    
 */
void Vector_Normalize_Positive(Value *p,int length,int pos) {
  
  Value *gcd;
  int i;
  
  gcd = Vector_Gcd(p,length);  	
  if (value_neg_p(p[pos]))
    value_oppose(*gcd,*gcd);            
  if(value_notone_p(*gcd))
    for(i=0; i<length; i++)
      value_division(p[i],p[i],*gcd);
  value_clear(*gcd);
  free(gcd);
  return;
} /* Vector_Normalize_Positive */

/* 
 * Reduce 'p' by operating binary function on its components successively 
 */
Value *Vector_Reduce(Value *p,unsigned length,Value *(*f)()) {
  
  Value *cp,*r;
  int i;
  
  r = (Value *)malloc(sizeof(Value));
  value_init(*r);
  cp = p;
  value_assign(*r,*cp);
  cp++;
  for(i=1;i<length;i++) {
    value_assign(*r, *(*f)(*r, *cp));
    cp++;
  }
  return r;
} /* Vector_Reduce */

/* 
 * Sort the components of a Vector 'vector' using Heap Sort. 
 */
void Vector_Sort(Value *vector,unsigned n) {
  
  int i, j;
  Value temp;
  Value *current_node=(Value *)0;
  Value *left_son,*right_son;

  value_init(temp);

  for (i=(n-1)/2;i>=0;i--) { 
    
    /* Phase 1 : build the heap */
    j=i;
    value_assign(temp,*(vector+i));
    
    /* While not a leaf */
    while (j<=(n-1)/2) {
      current_node = vector+j;
      left_son = vector+(j<<1)+1;

      /* If only one son */
      if ((j<<1)+2>=n) {
	if (value_lt(temp,*left_son)) {
	  value_assign(*current_node,*left_son);
	  j=(j<<1)+1;
	}
	else
	  break;
      }
      else {  
	
	/* If two sons */
	right_son=left_son+1;
	if (value_lt(*right_son,*left_son)) {
	  if (value_lt(temp,*left_son)) {
	    value_assign(*current_node,*left_son);
	    j=(j<<1)+1;
	  }
	  else
	    break;
	}
	else {
	  if (value_lt(temp,*right_son)) {
	    value_assign(*current_node,*right_son );
	    j=(j<<1)+2;
	  }
	  else
	    break;
	}
      }
    }
    value_assign(*current_node,temp);
  }
  for(i=n-1;i>0;i--) { 
    
    /* Phase 2 : sort the heap */
    value_assign(temp, *(vector+i));
    value_assign(*(vector+i),*vector);
    j=0;
    
    /* While not a leaf */
    while (j<i/2) {     
      current_node=vector+j;
      left_son=vector+(j<<1)+1;
      
      /* If only one son */
      if ((j<<1)+2>=i) { 		
	if (value_lt(temp,*left_son)) {
	  value_assign(*current_node,*left_son);
	  j=(j<<1)+1;
	}
	else
	  break;
      }
      else {
	
	/* If two sons */
	right_son=left_son+1;
	if (value_lt(*right_son,*left_son)) {
	  if (value_lt(temp,*left_son)) {
	    value_assign(*current_node,*left_son);
	    j=(j<<1)+1;
	  }
	  else
	    break;
	}
	else {
	  if (value_lt(temp,*right_son)) {
	    value_assign(*current_node,*right_son );
	    j=(j<<1)+2;
	  }
	  else
	    break;
	}
      }
    }
    value_assign(*current_node,temp);
  }
  value_clear(temp);
  return;
} /* Vector_Sort */








