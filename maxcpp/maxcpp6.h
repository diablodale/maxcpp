/*

Write MaxMSP (v5) objects in C++

-- general --

Allows the definition of a max object as a C++ class.

Most of the mechanics of typical max objects are encapsulated by the curiously recurring template pattern.

note: be careful to only include this header once, to avoid linker errors!
note: this probably won't work with virtuals -- and definitely won't work with multiple inheritance!

-- licence --
maxcpp is distributed under the permissive BSD-like MIT licence:

Copyright (c) 2009-2013 Graham Wakefield

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/

#ifndef MAXMSP_CPP_6_H
#define MAXMSP_CPP_6_H

#include "ext.h"
#include "ext_obex.h"
#include "ext_common.h"
#include "commonsyms.h"
#include "jpatcher_api.h"
#include "jgraphics.h"
#include "z_dsp.h"
#include "max.attr.h"

#include <new>

#define MAX_CPP_VERSION	0.600

// for A_NOTHING methods (e.g. bang):
#define REGISTER_METHOD(CLASS, METHOD)	class_addmethod(	\
						(t_class *)CLASS::m_class,						\
						(method)CLASS::MaxMethod<&CLASS::METHOD>::call, \
						#METHOD,										\
						0);

// for A_CANT methods (dblclick):
#define REGISTER_METHOD_CANT(CLASS, METHOD) class_addmethod(	\
						(t_class *)CLASS::m_class,							\
						(method)CLASS::MaxMethodCant<&CLASS::METHOD>::call, \
						#METHOD,		\
						A_CANT,			\
						0);

// for A_CANT methods (edclose):
#define REGISTER_METHOD_EDCLOSE(CLASS, METHOD)	class_addmethod(	\
						(t_class *)CLASS::m_class,								\
						(method)CLASS::MaxMethodEdClose<&CLASS::METHOD>::call,	\
						#METHOD,		\
						A_CANT,			\
						0);

// for A_CANT methods (assist):
#define REGISTER_METHOD_ASSIST(CLASS, METHOD) class_addmethod(	\
						(t_class *)CLASS::m_class,								\
						(method)CLASS::MaxMethodAssist<&CLASS::METHOD>::call, \
						#METHOD,		\
						A_CANT,			\
						0);

// for A_CANT methods (assist):
#define REGISTER_METHOD_LOADBANG(CLASS, METHOD) class_addmethod(    \
                        (t_class *)CLASS::m_class,                                \
                        (method)CLASS::MaxMethodLoadBang<&CLASS::METHOD>::call, \
                        #METHOD,        \
                        A_CANT,         \
                        0);

// for A_CANT methods (jsave)
#define REGISTER_METHOD_JSAVE(CLASS, METHOD)	class_addmethod(	\
						(t_class *)CLASS::m_class,								\
						(method)CLASS::MaxMethodJsave<&CLASS::METHOD>::call,	\
						#METHOD,		\
						A_CANT,			\
						0);

// for A_GIMME methods (t_symbol * s, long argc, void * argv):
#define REGISTER_METHOD_GIMME(CLASS, METHOD)	class_addmethod(	\
						(t_class *)CLASS::m_class,								\
						(method)CLASS::MaxMethodGimme<&CLASS::METHOD>::call,	\
						#METHOD,												\
						A_GIMME,												\
						0);

// for A_GIMMEBACK methods (t_symbol *s, long ac, t_atom *av, t_atom *rv):
#define REGISTER_METHOD_GIMMEBACK(CLASS, METHOD)	class_addmethod(	\
						(t_class *)CLASS::m_class,								\
						(method)CLASS::MaxMethodGimmeback<&CLASS::METHOD>::call,	\
						#METHOD,												\
						A_GIMMEBACK,												\
						0);

// for A_DEFSYM methods (t_symbol *s) (par ex, read) :
#define REGISTER_METHOD_DEFSYM(CLASS, METHOD) class_addmethod(	\
						(t_class *)CLASS::m_class,								\
						(method)CLASS::MaxMethodDefSym<&CLASS::METHOD>::call, \
						#METHOD,												\
						A_DEFSYM,												\
						0);

// for A_FLOAT methods (double v):
#define REGISTER_METHOD_FLOAT(CLASS, METHOD)	class_addmethod(	\
						(t_class *)CLASS::m_class,								\
						(method)CLASS::MaxMethodFloat<&CLASS::METHOD>::call,	\
						#METHOD,												\
						A_FLOAT,												\
						0);
// alternate for handling raw float/double data (since float is a reserved word in C++)
#define REGISTER_INLET_FLOAT(CLASS, METHOD)	class_addmethod(	\
						(t_class *)CLASS::m_class,								\
						(method)CLASS::MaxMethodFloat<&CLASS::METHOD>::call,	\
						"float",												\
						A_FLOAT,												\
						0);
// for A_INT methods (long v):
#define REGISTER_METHOD_LONG(CLASS, METHOD) class_addmethod(	\
						(t_class *)CLASS::m_class,							\
						(method)CLASS::MaxMethodLong<&CLASS::METHOD>::call, \
						#METHOD,											\
						A_LONG,												\
						0);
// alternate for handling raw int/long data (since int/long are reserved words in C++)
#define REGISTER_INLET_LONG(CLASS, METHOD) class_addmethod(	\
						(t_class *)CLASS::m_class,							\
						(method)CLASS::MaxMethodLong<&CLASS::METHOD>::call, \
						"int",											\
						A_LONG,												\
						0);

// for A_CANT method (notify)
#define REGISTER_METHOD_NOTIFY(CLASS) class_addmethod(									\
						(t_class *)CLASS::m_class,								\
						(method)CLASS::MaxMethodNotify<&CLASS::notify>::call,	\
						"notify",												\
						A_CANT,													\
						0);
// JBOX Methods
// for A_CANT methods (mouse):
#define REGISTER_METHOD_MOUSE(CLASS, METHOD) class_addmethod(							\
						(t_class *)CLASS::m_class,								\
						(method)CLASS::MaxMethodMouse<&CLASS::METHOD>::call,	\
						#METHOD,												\
						A_CANT,													\
						0);

// used for clocks, qelem, and other no parameter delegate methods
#define TO_METHOD_NONE(CLASS, METHOD) ((method)CLASS::MaxMethodNone<&CLASS::METHOD>::call)

// used for defer, defer_medium, defer_low, schedule, schedule_delay, etc.
#define TO_METHOD_TASK(CLASS, METHOD) ((method)CLASS::MaxMethodTask<&CLASS::METHOD>::call)

// used for attr accessors
#define TO_METHOD_GET(CLASS, METHOD) ((method)CLASS::MaxMethodAccessorGet<&CLASS::METHOD>::call)
#define TO_METHOD_SET(CLASS, METHOD) ((method)MaxCppBase<CLASS>::MaxMethodAccessorSet<&CLASS::METHOD>::call)

// for DSP perform function
#define REGISTER_PERFORM(CLASS, METHOD) object_method( \
						dsp64, \
						gensym("dsp_add64"), \
						(t_pxobject *)this, \
						MaxMethodPerform64<&CLASS::METHOD>::call,\
						0, \
						nullptr);

// for DSP state notify
#define REGISTER_METHOD_DSPSTATE(CLASS, METHOD) class_addmethod(				\
						(t_class *)CLASS::m_class,								\
						(method)CLASS::MaxMethodDspState<&CLASS::METHOD>::call,	\
						"dspstate",												\
						A_CANT,													\
						0);

// a purely static base class for Max and MSP objects:
template <typename T>
class MaxCppBase {
public:

	static t_class * m_class;

	// template functors to forward Max messages to class methods:
	typedef void (T::*maxmethodgimme)(const long inlet, const t_symbol * const s, const long ac, const t_atom * const av);
	template<maxmethodgimme F>
	struct MaxMethodGimme {
		static void call(T * x, const t_symbol * const s, const long ac, const t_atom * const av) { ((x)->*F)(proxy_getinlet((t_object *)x), s, ac, av); }
	};

	typedef t_max_err (T::*maxmethodgimmeback)(const t_symbol * const s, const long ac, const t_atom * const av, t_atom * const rv);
	template<maxmethodgimmeback F>
	struct MaxMethodGimmeback {
		static t_max_err call(T * x, const t_symbol * const s, const long ac, const t_atom * const av, t_atom * const rv) { return ((x)->*F)(s, ac, av, rv); }
	};

	typedef void (T::*maxmethod)(const long inlet);
	template<maxmethod F>
	struct MaxMethod {
		static void call(T * x) { ((x)->*F)(proxy_getinlet((t_object *)x)); }
	};

	//A_CANT for dblclick
	typedef void (T::*maxmethodcant)(const long inlet);
	template<maxmethodcant F>
	struct MaxMethodCant {
		static void call(T * x) { ((x)->*F)(proxy_getinlet((t_object *)x)); }
	};

	//A_CANT for drag
	typedef long (T::*maxmethoddrag)(t_object *drag, t_object *view);
	template<maxmethoddrag F>
	struct MaxMethodDrag {
		static long call(T * x, t_object *drag, t_object *view) { return ((x)->*F)(drag, view); }
	};

	//A_CANT for edclose and edsave
	typedef void (T::*maxmethodedclose)(const long inlet, char** const text, const long size);
	template<maxmethodedclose F>
	struct MaxMethodEdClose {
		static void call(T * x, char** const text, const long size) { ((x)->*F)(proxy_getinlet((t_object *)x), text, size); }
	};

	//A_CANT for assist
	typedef void (T::*maxmethodassist)(void *b, const long io, const long index, char * const dst);
	template<maxmethodassist F>
	struct MaxMethodAssist {
		static void call(T * x, void *b, const long io, const long index, char * const dst) { ((x)->*F)(b, io, index, dst); }
	};

    //A_CANT for loadbang
    typedef void (T::*maxmethodloadbang)(void *b);
    template<maxmethodloadbang F>
    struct MaxMethodLoadBang {
        static void call(T * x, void *b) { ((x)->*F)(b); }
    };

	//A_CANT for jsave
	typedef void (T::*maxmethodjsave)(t_dictionary *d);
	template<maxmethodjsave F>
	struct MaxMethodJsave {
		static void call(T * x, t_dictionary *d) { ((x)->*F)(d); }
	};

	//proxy_getinlet((t_object *)x),
	typedef void (T::*maxmethodlong)(const long inlet, const long v);
	template<maxmethodlong F>
	struct MaxMethodLong {
		static void call(T * x, const long v) { ((x)->*F)(proxy_getinlet((t_object *)x), v); }
	};

	//Template que j'ai rajoute for pouvoir faire A_DEFSYM(t_symbol *s)
	typedef void (T::*maxmethoddefsym)(const long inlet, const t_symbol * const s);
	template<maxmethoddefsym F>
	struct MaxMethodDefSym {
		static void call(T * x, const t_symbol * const s) { ((x)->*F)(proxy_getinlet((t_object *)x), s); }
	};

	typedef void (T::*maxmethodfloat)(const long inlet, const double v);
	template<maxmethodfloat F>
	struct MaxMethodFloat {
		static void call(T * x, const double v) { ((x)->*F)(proxy_getinlet((t_object *)x), v); }
	};

	//A_CANT for notify
	typedef t_max_err (T::*maxmethodnotify)(const t_symbol * const s, const t_symbol *msg, void * const sender, void * const data);
	template<maxmethodnotify F>
	struct MaxMethodNotify{
		static t_max_err call(T * x, const t_symbol * const s, const t_symbol *msg, void * const sender, void * const data) {return ((x)->*F)(s, msg, sender, data); }
	};

	//JBOX methods
	//A_CANT for paint
	typedef void (T::*maxmethodpaint)(t_object *view);
	template<maxmethodpaint F>
	struct MaxMethodPaint {
		static void call(T * x, t_object *view) { ((x)->*F)(view); }
	};

	//A_CANT mouse
	typedef void (T::*maxmethodmouse)(t_object *patcherview, t_pt pt, long modifiers);
	template<maxmethodmouse F>
	struct MaxMethodMouse {
		static void call(T * x,t_object *patcherview, t_pt pt, long modifiers) { ((x)->*F)(patcherview,pt,modifiers); }
	};

	///////////////////////////////////////////////////////////////////////////////////
	typedef void (T::*maxmethodnone)();
	template<maxmethodnone F>
	struct MaxMethodNone {
		static void call(T * x) { ((x)->*F)(); }
	};
	// (method) task
	typedef void (T::*maxmethodtask)(const t_symbol * const s, const long ac, const t_atom * const av);
	template<maxmethodtask F>
	struct MaxMethodTask {
		static void call(T * x, const t_symbol * const s, const long ac, const t_atom * const av) { ((x)->*F)(s, ac, av); }
	};
	// (method) attr get accessor
	typedef t_max_err (T::*maxmethodaccessorget)(long * const ac, t_atom ** const av);
	template<maxmethodaccessorget F>
	struct MaxMethodAccessorGet {
		static t_max_err call(T * x, void *attr, long * const ac, t_atom ** const av) { return ((x)->*F)(ac, av); }
	};
	// (method) attr set accessor
	typedef t_max_err (T::*maxmethodaccessorset)(const long ac, const t_atom * const av);
	template<maxmethodaccessorset F>
	struct MaxMethodAccessorSet {
		static t_max_err call(T * x, void *attr, const long ac, const t_atom * const av) { return ((x)->*F)(ac, av); }
	};

};

// note: only include this file once to prevent linker errors!
template<typename T> t_class * MaxCppBase<T>::m_class = nullptr;

// inherit from this one for non-audio objects
template <typename T>
class MaxCpp6 : public MaxCppBase<T> {
public:
	t_object m_ob;
	void **	m_outlets;
	void **	m_inletproxies;
	long m_whichinlet;

	static t_class * makeMaxClass(const char * const classname) {
		if (!_common_symbols) common_symbols_init();
		t_class * const c = class_new(classname, (method)MaxCpp6<T>::maxcpp_create, (method)MaxCpp6<T>::maxcpp_destroy, sizeof(T), 0, A_GIMME, 0);
		class_register(CLASS_BOX, c);
		MaxCppBase<T>::m_class = c;
		return c;
	}

	static void * maxcpp_create(const t_symbol * const sym, const long ac, const t_atom * const av) {
		void * const x = object_alloc(MaxCppBase<T>::m_class);
		new(x) T(sym, ac, av);
		return (T *)x;
	}

	static void maxcpp_destroy(t_object * const x) {
		T * const t = (T *)x;
		t->~T();

		// release manually allocated memory and inlet proxies
		const unsigned long numinletproxies = sysmem_ptrsize(t->m_inletproxies)/sizeof(void*);
		for (unsigned int i=0; i < numinletproxies; ++i) {
		   object_free(t->m_inletproxies[i]);
		}
		sysmem_freeptr(t->m_inletproxies);
		sysmem_freeptr(t->m_outlets);
	}

	void setupIO(const unsigned int numinlets = 1, const unsigned int numoutlets = 1) {
		if (numinlets > 0) {
			const unsigned int numproxies = numinlets - 1;
			m_inletproxies = (void **)sysmem_newptr(sizeof(void *) * numproxies);
			for (unsigned int i=numproxies; 0 < i; i--)
				m_inletproxies[i - 1] = proxy_new(this, i, &this->m_whichinlet); // generic inlet
		}
		m_outlets = (void **)sysmem_newptr(sizeof(void *) * numoutlets);
		for (unsigned int i=0; i<numoutlets; ++i)
			m_outlets[numoutlets - i - 1] = outlet_new(this, NULL); // generic outlet
	}

	// C++ operator overload to treat MaxCpp6 objects as t_objects
	operator t_object & () { return m_ob; }
};

// inherit from this one for audio objects
template <typename T>
class MspCpp6 : public MaxCppBase<T> {
public:
	t_pxobject m_ob;
	void **	m_outlets;
	void **	m_inletproxies;
	long m_whichinlet;

	typedef void (T::*maxmethod_perform64)(double * const * const ins, const long numins, double * const * const outs, const long numouts, const long sampleframes);
	template<maxmethod_perform64 F>
	struct MaxMethodPerform64 {
		static void call(T * const x, t_object * const dsp64, double * const * const ins, const long numins, double * const * const outs, const long numouts, const long sampleframes, const long flags, void * const userparam) {
			((x)->*F)(ins, numins, outs, numouts, sampleframes);
		}
	};

	//A_CANT for DSP state
	typedef void (T::*maxmethoddspstate)(const long dspstate);
	template<maxmethoddspstate F>
	struct MaxMethodDspState {
		static void call(T * const x, const long dspstate) { ((x)->*F)(dspstate); }
	};

	static t_class * makeMaxClass(const char * const classname) {
		if (!_common_symbols) common_symbols_init();
		t_class * const c = class_new(classname, (method)MspCpp6<T>::maxcpp_create, (method)MspCpp6<T>::maxcpp_destroy, sizeof(T), NULL, A_GIMME, 0);
		class_dspinit(c);

		class_addmethod(c, (method)MspCpp6<T>::maxcpp_dsp64, "dsp64", A_CANT, 0);

		class_register(CLASS_BOX, c);
		MaxCppBase<T>::m_class = c;
		return c;
	}

	static void * maxcpp_create(const t_symbol * const sym, const long ac, const t_atom * const av) {
		void * const x = object_alloc(MaxCppBase<T>::m_class);
		new(x) T(sym, ac, av);
		return (T *)x;
	}

	void setupIO(const unsigned int signal_inlets, const unsigned int signal_outlets, const unsigned int non_signal_inlets = 0, const unsigned int non_signal_outlets = 0) {
		// prevent recycling of inputs for outputs
		m_ob.z_misc = Z_NO_INPLACE;

		// create non-signal inlets
		if (non_signal_inlets > 0) {
			const int numproxies = non_signal_inlets - (signal_inlets ? 0 : 1);
			m_inletproxies = (void **)sysmem_newptr(sizeof(void *) * numproxies);
			unsigned int end_inlet_index = signal_inlets + non_signal_inlets - 1;
			for (int i = 0; i < numproxies; ++i) {
				m_inletproxies[numproxies - i - 1] = proxy_new(this, end_inlet_index - i, &this->m_whichinlet); // generic inlet
			}
		}

		// create signal inlets
		dsp_setup((t_pxobject *)this, signal_inlets);

		// create non-signal outlets
		unsigned int idx_outlet = signal_outlets + non_signal_outlets;
		if (idx_outlet) {
			m_outlets = (void **)sysmem_newptr(sizeof(void *) * idx_outlet);
			--idx_outlet;	// offset to 0-base
			for (unsigned int i = 0; i < non_signal_outlets; ++i) {
				m_outlets[idx_outlet - i] = outlet_new(this, NULL); // generic outlet
			}

			// create signal outlets
			idx_outlet = signal_outlets - 1;
			for (unsigned int i = 0; i < signal_outlets; ++i) {
				m_outlets[idx_outlet - i] = outlet_new((t_object *)this, "signal");
			}
		}
	}

	static void maxcpp_destroy(t_pxobject * const x) {
		dsp_free(x);
		T * const t = (T *)x;
		t->~T();

		// release manually allocated memory and inlet proxies
		const unsigned long numinletproxies = sysmem_ptrsize(t->m_inletproxies)/sizeof(void*);
		for (unsigned int i=0; i < numinletproxies; ++i) {
		   object_free(t->m_inletproxies[i]);
		}
		sysmem_freeptr(t->m_inletproxies);
		sysmem_freeptr(t->m_outlets);
	}

	static void maxcpp_dsp64(t_pxobject * const x, t_object * const dsp64, const short * const count, const double samplerate, const long maxvectorsize, const long flags) {
		((T *)x)->dsp(dsp64, count, samplerate, maxvectorsize, flags);
	}

	// stub functions in case the user doesn't supply them:
	void dsp(t_object * const dsp64, const short * const count, const double samplerate, const long maxvectorsize, const long flags) {
		REGISTER_PERFORM(T, perform);
	}
	void perform(double * const * const ins, const long numins, double * const * const outs, const long numouts, const long sampleframes) {}

	// C++ operator overload to treat MaxMsp6 objects as t_objects
	operator t_object & () { return m_ob.z_ob; }
	operator t_pxobject & () { return m_ob; }
};

// inherit from this one for jbox objects
template <typename T>
class JboxCpp6 : public MaxCppBase<T> {
public:
	t_jbox m_ob;
	long m_flags;
	void **	m_outlets;
	void **	m_inletproxies;
	long m_whichinlet;
	long m_inlet_count;
	long m_outlet_count;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	static t_class* makeMaxClass(const char *classname,long jboxflags = 0,const char *defaultrect = "0 0 100 100"){
		if (!_common_symbols) common_symbols_init();
		t_class *c = class_new(classname,(method)JboxCpp6<T>::maxcpp_create,(method)JboxCpp6<T>::maxcpp_destroy,sizeof(T),0,A_GIMME,0);
		c->c_flags |= CLASS_FLAG_NEWDICTIONARY;

		jbox_initclass(c,jboxflags);

		class_addmethod(c, (method)JboxCpp6<T>::notify_class,"notify", A_CANT, 0);
		class_addmethod(c, (method)JboxCpp6<T>::paint_class, "paint", A_CANT, 0);

		// default size
		CLASS_ATTR_DEFAULT(c, "patching_rect", 0, defaultrect);

		class_register(CLASS_BOX, c);
		MaxCppBase<T>::m_class = c;

		return c;
	}

	static void* maxcpp_create(t_symbol * sym, long ac, t_atom * av){
		void *x = object_alloc(MaxCppBase<T>::m_class);
		((T *)x)->m_flags = 0 ;
		new(x) T();

		t_dictionary *d = object_dictionaryarg(ac,av) ;

		jbox_new(&((T *)x)->m_ob, ((T *)x)->m_flags , ac, av);
		((T *)x)->m_ob.b_firstin = (t_object*)(T *)x;

		((T *)x)->setupIOclass();

		attr_dictionary_process((T *)x, d);
		jbox_ready(&((T *)x)->m_ob);

		return (T *)x;
	}

	static void maxcpp_destroy(t_jbox * x){
		jbox_free(&t->m_ob);
		T * const t = (T *)x;
		t->~T();

		// release manually allocated memory and inlet proxies
		const unsigned long numinletproxies = sysmem_ptrsize(t->m_inletproxies)/sizeof(void*);
		for (unsigned int i=0; i < numinletproxies; ++i) {
		   object_free(t->m_inletproxies[i]);
		}
		sysmem_freeptr(t->m_inletproxies);
		sysmem_freeptr(t->m_outlets);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void setupJbox(long jboxflags){
		m_flags |= jboxflags ;
	}

	void setupIO(unsigned int numinlets = 1, unsigned int numoutlets = 1) {
		m_inlet_count = numinlets;
		m_outlet_count = numoutlets;
	}

	void setupIOclass(){
		if (m_inlet_count > 0) {
			unsigned int numproxies = m_inlet_count - 1;
			m_inletproxies = (void **)sysmem_newptr(sizeof(void *) * numproxies);
			for (unsigned int i=0; i<numproxies; ++i)
				m_inletproxies[i] = proxy_new(this, i+1, &this->m_whichinlet); // generic inlet
		}
		m_outlets = (void **)sysmem_newptr(sizeof(void *) * m_outlet_count);
		for (unsigned int i=0; i<m_outlet_count; ++i)
			m_outlets[m_outlet_count - i - 1] = outlet_new(this, NULL); // generic outlet
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	static void paint_class(t_object *x,t_object *view){
		t_rect rect;
		t_jgraphics *g = (t_jgraphics*)patcherview_get_jgraphics(view);
		jbox_get_rect_for_view(x, view, &rect);
		((T *)x)->paint(g, view, rect);
	}

	void paint(t_jgraphics *g, t_object *view , t_rect rect){}


	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	static t_max_err notify_class(t_object *x,symbol *s, symbol *msg, void *sender, void *data){
		return ((T *)x)->notify(s, msg, sender, data);
	}

	t_max_err notify(symbol *s, symbol *msg, void *sender, void *data){
		return jbox_notify((t_jbox *)this, s, msg, sender, data);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// C++ operator overload to treat JboxCpp6 objects as t_jbox and t_object
	operator t_jbox & () { return m_ob; }
	operator t_object & () { return m_ob.b_ob; }
};

#endif
