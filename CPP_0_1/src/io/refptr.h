/***************************************************************************
 *   Copyright (C) 2006 by Jonathan Sieber                                 *
 *   jonathan_sieber@yahoo.de                                              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef REFPTR_H
#define REFPTR_H

#include <cassert>
#include "platform.h"
#include "io/resource.h"

template <class T>
class RefPtr
{
public:
	RefPtr() FORCEINLINE 
		: m_ref(0)
	{
	}
	
	RefPtr(T* ptr) FORCEINLINE
		: m_ref(ptr)
	{
		if (m_ref)
			m_ref->addRef();
	}
	
	RefPtr(const RefPtr& ptr) FORCEINLINE
	{
		m_ref = ptr.m_ref;
		if (m_ref)
			m_ref->addRef();
	}
	
	~RefPtr()
	{
		if (m_ref)
			m_ref->removeRef();
	}

	template <class S>
	RefPtr<S> downcast() const
	{
		return RefPtr<S>(static_cast<S*>(m_ref));
	}

	bool isNull() const
	{
		return NULL == m_ref;
	}

	bool isNotNull() const
	{
		return NULL != m_ref;
	}

	RefPtr& operator = (const RefPtr& ref) FORCEINLINE
	{
		if (m_ref != ref.m_ref)
		{
			if (m_ref)
				m_ref->removeRef();
			m_ref = ref.m_ref;
			if (m_ref)
				m_ref->addRef();
		}
		return *this;
	}
	
	inline T& operator * () const FORCEINLINE
	{
		assert(m_ref);
		return *m_ref;
	}
	
	inline T* operator -> () const FORCEINLINE
	{
		assert(m_ref);
		return m_ref;
	}

	inline bool operator == (const RefPtr<T>& y) const
	{
		return m_ref == y.m_ref;
	}

	inline bool operator != (const RefPtr<T>& y) const
	{
		return m_ref != y.m_ref;
	}

	inline bool operator < (const RefPtr<T>& y) const
	{
		return m_ref < y.m_ref;
	}
	
	inline bool operator > (const RefPtr<T>& y) const
	{
		return m_ref > y.m_ref;
	}

	inline bool operator <= (const RefPtr<T>& y) const
	{
		return m_ref <= y.m_ref;
	}

	inline bool operator >= (const RefPtr<T>& y) const
	{
		return m_ref >= y.m_ref;
	}

private:
	T* m_ref;
};


#endif
