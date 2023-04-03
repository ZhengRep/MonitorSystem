#pragma once

template<class T> T& unmove(T&& t) { return t; }