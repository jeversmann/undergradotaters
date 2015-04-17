// $Id: handle.h,v 1.3 2003/08/07 23:14:22 pnav Exp $
// ------------------
// Life4/Handle.h
// Copyright (C) 2001
// Glenn P. Downing
// ------------------

#ifndef Handle_h
#define Handle_h

// --------
// Includes
// --------

#include <cassert> // assert

// ----------
// Namespaces
// ----------

using namespace std;

// ---------
// Handle<T>
// ---------

template <class T>
class Handle {
    public:
        // ------------
        // Constructors
        // ------------

        Handle () {
            p = 0;}

        Handle (T * rhs) {
            p = rhs;}

        Handle (const Handle& rhs) {
            if (!rhs.p)
                p = 0;
            else
                p = rhs.p->clone();}

        // ----------
        // Destructor
        // ----------

        ~Handle () {
            delete p;}

        // ---------
        // Operators
        // ---------

        Handle& operator = (const Handle& rhs) {
            if (this != &rhs) {
                delete p;

                if (!rhs.p)
                    p = 0;
                else
                    p = rhs.p->clone();}

            return *this;}

        void replace(T * new_ptr) {
          delete p;
          p = new_ptr;
        }

        operator void* () const {
            return p;}

        T& operator * () const {
            assert(p);
            return *p;}

        T* operator -> () const {
            assert(p);
            return p;}

        // -------
        // Methods
        // -------

        void clear () {
            delete p;
            p = 0;}

        template <class U> void put_pointer_in( U * & ptr) {
          ptr = (U *) p;
        }

    private:
        // ----
        // Data
        // ----

        T* p;};

#endif // // Handle_h

