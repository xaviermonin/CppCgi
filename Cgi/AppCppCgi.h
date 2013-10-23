#ifndef _APP_CPPCGI_H_
#define _APP_CPPCGI_H_

#include "App.h"

#include "Session.h"
#include "Post.h"
#include "Cookie.h"
#include "Get.h"

namespace CppCgi {

template<class Cible, class Env=EnvironnementCgi>
class AppCppCgi : public App<Cible, Env>
{
public:
	AppCppCgi(){}
	virtual ~AppCppCgi(){}

	virtual void preClient() { }

	virtual void postClient()
	{
		Post::libererInstance();
		Get::libererInstance();
		Session::libererInstance();
		Cookie::libererInstance();
	}
};

} // namespace CppCgi

#endif
