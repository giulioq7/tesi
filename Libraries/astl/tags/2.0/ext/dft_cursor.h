#ifndef ASTL_TRANSDUCER_CURSOR
#define ASTL_TRANSDUCER_CURSOR

// Fichiers ASTL modifi�s:
// alphabet.h
// cursor.h

#include <cursor.h>

// Requirements:
// class DFT is a model of subsequential finite transducer
// class OutputIterator is a model of output iterator 
// the output alphabet must be assignable

// Au cas o� on ne veut pas se servir d'it�rateur de sortie:
template <class Alphabet>
struct bogus_output_iterator 
{
  bogus_output_iterator& operator* () {
    return *this;
  }
  bogus_output_iterator& operator= (const Alphabet &) {
    return *this;
  }
  bogus_output_iterator& operator++ () {
    return *this;
  }
};

template <class DFT, 
          class OutputIterator = bogus_output_iterator<typename DFT::Alphabet2> >
class dft_cursor : public cursor<DFT>
{
public:
  typedef dft_cursor              self;
  typedef typename DFT::Alphabet  Alphabet;
  typedef typename DFT::Alphabet2 Alphabet2;
  typedef cursor<DFT>             super;
  OutputIterator                  out;

  dft_cursor()
    : super()
  { }

  dft_cursor(const DFT &dft)
    : super(dft)
  { }

  dft_cursor(const DFT &dft, const OutputIterator &x)
    : super(dft), out(x)
  { }

  dft_cursor(const DFT &dft, State p, const OutputIterator &x)
    : super(dft, q), out(x)
  { }

  // Cette m�thode stocke automatiquement le symbole �mis gr�ce � l'it�rateur
  // de sortie fourni � la construction (permet de voir le DFT comme un DFA):
  bool forward(const Alphabet &a) {
    return forward(a, out);
  }

  // L'appelant fournit un it�rateur de sortie pour le symbole �mis (il peut �tre
  // d'un type diff�rent de celui du param�tre d'instanciation):
  template <class AnyOutputIterator>
  bool forward(const Alphabet &a, AnyOutputIterator &x) 
  {
    const pair<State, Alphabet2> &p = dfa->delta3(q, a);
    q = p.first;
    if (q != dfa->null_state) {
      *x = p.second;
      ++x;
      return true;
    }
    return false;
  }

  // Le symbole �mis est stock� dans la r�f�rence 'output':
  bool forward(const Alphabet &a, Alphabet2 &output) {
    q = dfa->delta3(q, a, output);
    return !sink();
  }

  self& operator= (State p) {
    q = p;
    return *this;
  }
};

template <class DFT, 
	  class OutputIterator = bogus_output_iterator<typename DFT::Alphabet2> >
class dft_forward_cursor : public forward_cursor<DFT>
{
public:
  typedef dft_forward_cursor      self;
  typedef typename DFT::Alphabet  Alphabet;
  typedef typename DFT::Alphabet2 Alphabet2;
  typedef forward_cursor<DFT>     super;
  OutputIterator                  out;

  dft_forward_cursor()
    : super()
  { }

  dft_forward_cursor(const DFT &dft)
    : super(dft)
  { }

  dft_forward_cursor(const DFT &dft, const OutputIterator &x)
    : super(dft), out(x)
  { }

  dft_forward_cursor(const DFT &dft, State p, const OutputIterator &x)
    : super(dft, q), out(x)
  { }

  pair<const Alphabet, Alphabet2> letter() const {
    return (*t).first;
  }

  // Cette m�thode stocke automatiquement le symbole �mis gr�ce � l'it�rateur
  // de sortie fourni � la construction 
  // Warning: ne pas utiliser si l'it�rateur n'a pas �t� donn� � la construction !!!!
  bool forward(const Alphabet &a) {
    return forward(a, out);
  }

  // L'appelant fournit un it�rateur de sortie pour le symbole �mis (il peut �tre
  // d'un type diff�rent de celui du param�tre d'instanciation):
  template <class AnyOutputIterator>
  bool forward(const Alphabet &a, AnyOutputIterator &x) 
  {
    const pair<State, Alphabet2> &p = dfa->delta3(q, a);
    q = p.first;
    if (q != dfa->null_state) {
      *x = p.second;
      ++x;
      return true;
    }
    return false;
  }

  // Le symbole �mis est stock� dans la r�f�rence 'output':
  bool forward(const Alphabet &a, Alphabet2 &output) {
    q = dfa->delta3(q, a, output);
    return !sink();
  }

  // Stocke automatiquement le symbole �mis:
  void forward() {
    forward(out);
  }

  template <class AnyOutputIterator>
  void forward(AnyOutputIterator &x) {
    *x = (*transition).first.second;
    ++x;
    super::forward();
  }

  void forward(Alphabet2 &output) {
    output = (*transition).first.second;
    super::forward();
  }

  self& operator= (State p) {
    q = p;
    return *this;
  }
};

// Curseur de transducteur avec transitions par d�faut �tiquet�es par def_letter
// Lorsque la transition n'est pas d�finie, le curseur suit la transition par d�faut
// et �met donc le symbole correspondant

template <class DFTForwardCursor>
class def_trans_dft_cursor : public DFTForwardCursor
{
public:
  typedef DFTForwardCursor          super;
  typedef def_trans_dft_cursor      self;
  typedef typename super::State     State;
  typedef typename super::Alphabet  Alphabet;
  typedef typename super::Alphabet2 Alphabet2;
	
protected:
  Alphabet def_letter;

public:
  def_trans_dft_cursor(const DFTForwardCursor &x, const Alphabet &default_letter = 0)
    : super(x), def_letter(default_letter)
  { }

  bool forward(const Alphabet &a) {
    if (find(a))
      return super::forward();
    else
      return super::forward(def_letter);
  }

  template <class OutputIterator>
  bool forward(const Alphabet &a, OutputIterator &x) {
    if (find(a))
      return super::forward(x);
    else
      return super::forward(def_letter, x);
  }
	
  bool forward(const Alphabet &a, Alphabet2 &output)
  {
    if (find(a))
      return super::forward(output);
    else
      return super::forward(def_letter, output);
  }

  self& operator= (State p) {
    super::operator=(p);
    return *this;
  }
};

template <class DFT>
class underlying_dfa_cursor : public forward_cursor<DFT>
{
public:
  typedef underlying_dfa_cursor      self;
  typedef pair<typename DFT::Alphabet, typename DFT::Alphabet2> Alphabet;
  typedef forward_cursor<DFT>        super;

  underlying_dfa_cursor()
    : super()
  { }

  underlying_dfa_cursor(const DFT &dft)
    : super(dft)
  { }

  underlying_dfa_cursor(const DFT &dft, State p)
    : super(dft, p)
  { }

  Alphabet letter() const {
    return (*transition).first;
  }

  self& operator= (State p) {
    q = p;
    return *this;
  }
};

// Un adaptateur de curseur de transducteur avec une fonction d'�mission sp�ciale rho
// rho : F -> Alphabet2
// Associe un �tat final q � un symbole d'�mission e. La fonction d'�mission choisie
// utilise le tag de l'�tat final
// Si ce symbole a la valeur sp�ciale, le symbole d'entr�e est �mis en sortie (identit�)
//
// Ce type de curseur est donc param�tr� par 3 choses:
// 1. Le curseur � adapter
// 2. La fonction d'�mission (DFTForwardCursor&, 
// 3. La valeur sp�ciale

/*
  template <class DFTForwardCursor, class ObjectFunction>
  class rho_dft_cursor : public DFTForwardCursor
  {
  public:
  typedef DFTCursor                 super;
  typedef rho_dft_cursor            self;
  typedef typename super::State     State;
  typedef typename super::Alphabet  Alphabet;
  typedef typename super::Alphabet2 Alphabet2;
	
  protected:
  Alphabet2 identity_letter;

  public:
  rho_dft_cursor(const DFTCursor &x, const Alphabet2 &special_letter)
  : super(x), identity_letter(special_letter)
  { }

  bool forward(const Alphabet &a) {
  if (super::forward(a)) {
  if (src_final()) {
  if (src_tag() 

  else
  return super::forward(def_letter);
  }

  template <class OutputIterator>
  bool forward(const Alphabet &a, OutputIterator x) {
  if (find(a))
  return super::forward(x);
  else
  return super::forward(def_letter, x);
  }
	
  bool forward(const Alphabet &a, Alphabet2 &output)
  {
  if (find(a))
  return super::forward(output);
  else
  return super::forward(def_letter, output);
  }

  self& operator= (State p) {
  super::operator=(p);
  return *this;
  }
  };
*/

/*
// Un curseur de transducteur avec un symbole d'�mission sp�cial (pr�cis� � la construction)
// correspondant � l'identit�
template <class DFTCursor>
class dft_cursor_with_id : public DFTCursor
{
public:
typedef DFTCursor                 super;
typedef def_trans_dft_cursor      self;
typedef typename super::State     State;
typedef typename super::Alphabet  Alphabet;
typedef typename super::Alphabet2 Alphabet2;
	
protected:
Alphabet def_letter;

public:
def_trans_dft_cursor(const DFTCursor &x, const Alphabet &default_letter = 0)
: super(x), def_letter(default_letter)
{ }

 bool forward(const Alphabet &a) {
   if (exists(a))
     return super::forward(a);
   else
     return super::forward(def_letter);
 }

 template <class OutputIterator>
   bool forward(const Alphabet &a, OutputIterator x) {
   if (exists(a))
     return super::forward(a, x);
   else
     return super::forward(def_letter, x);
 }
	
 bool forward(const Alphabet &a, Alphabet2 &x)
   {
     if (exists(a))
       return super::forward(a, x);
     else
       return super::forward(def_letter, x);
   }

 self& operator= (State p) {
   super::operator=(p);
   return *this;
 }
};
*/

#endif // ASTL_TRANSDUCER_CURSOR
