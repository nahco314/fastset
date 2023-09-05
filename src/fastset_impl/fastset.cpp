#pragma GCC target("avx2")
#pragma GCC optimize("Ofast")
#pragma GCC optimize("unroll-loops")


#include "hpy.h"
#include <Python.h>  // for PyUnicode_FromFormat in repr
#include <stdio.h>
#include <vector>
#include <iostream>


#define dup(x) HPy_Dup(_g_ctx, x)
#define close(x) HPy_Close(_g_ctx, x)
#define None dup(_g_ctx->h_None)
#define as_py(x) HPyLong_FromLong(_g_ctx, (long)x)
#define as_c(x) HPyLong_AsLong(_g_ctx, x)

using ll = long long;

static HPyContext* _g_ctx;

#ifdef __GNUC__
    #include <ext/pb_ds/assoc_container.hpp>
    #include <ext/pb_ds/tree_policy.hpp>
    using namespace std;
    using namespace __gnu_pbds;
    const static auto comp = less<ll>();
    using pb_set = tree<
        ll,
        null_type,
        decltype(comp),
        rb_tree_tag,
        tree_order_statistics_node_update
    >;
#else
    #include <set>
    using namespace std;
    using pb_set = set<ll>;
#endif


struct Set4PyLong {
    pb_set st;
    pb_set::iterator it;
    Set4PyLong() : st(comp), it(st.begin()) {}
    Set4PyLong(vector<int> vec) : st(vec.begin(), vec.end(), comp), it(st.begin()) {}
    Set4PyLong(const Set4PyLong& other) : st(other.st), it(st.begin()) {}

    bool add(HPy x) {
        ll x_c = as_c(x);

        const auto& r = st.insert(x_c);

        it = r.first;
        if (r.second) {
            return true;
        } else {
            return false;
        }
    }

    HPy remove(HPy x) {
        ll x_c = as_c(x);

        it = st.find(x_c);
        if (it == st.end()) return HPyErr_SetObject(_g_ctx, _g_ctx->h_KeyError, x);
        it = st.erase(it);
        if (it == st.end()) return None;
        return as_py(*it);
    }

    HPy search_higher_equal(HPy x) {
        ll x_c = as_c(x);

        it = st.lower_bound(x_c);
        if (it == st.end()) return None;
        return as_py(*it);
    }

    HPy min() {
        if (st.size() == 0)
            return HPyErr_SetString(_g_ctx, _g_ctx->h_IndexError, "min from an empty set");
        it = st.begin();
        return as_py(*it);
    }

    HPy max() {
        if (st.size() == 0)
            return HPyErr_SetString(_g_ctx, _g_ctx->h_IndexError, "max from an empty set");
        it = prev(st.end());
        return as_py(*it);
    }

    HPy pop_min() {
        if (st.size() == 0)
            return HPyErr_SetString(_g_ctx, _g_ctx->h_IndexError, "pop_min from an empty set");
        it = st.begin();
        HPy res = as_py(*it);
        it = st.erase(it);
        return res;
    }

    HPy pop_max() {
        if (st.size() == 0)
            return HPyErr_SetString(_g_ctx, _g_ctx->h_IndexError, "pop_max from an empty set");
        it = prev(st.end());
        HPy res = as_py(*it);
        it = st.erase(it);
        return res;
    }

    size_t len() {
        return st.size();
    }

    HPy iter_next() {
        if (it == st.end()) return None;
        if (++it == st.end()) return None;
        return as_py(*it);
    }

    HPy iter_prev() {
        if (it == st.begin()) return None;
        return as_py(*--it);
    }

    HPy to_list() {
        HPyListBuilder builder = HPyListBuilder_New(_g_ctx, len());
        int i = 0;
        for (ll p : st) {
            HPyListBuilder_Set(_g_ctx, builder, i++, as_py(p));
        }
        return HPyListBuilder_Build(_g_ctx, builder);
    }

    HPy get() {
        if (it == st.end()) return None;
        return as_py(*it);
    }

    HPy erase() {
        if (it == st.end()) return HPyErr_SetString(_g_ctx, _g_ctx->h_IndexError, "erase end");
        it = st.erase(it);
        if(it == st.end()) return None;
        return as_py(*it);
    }

    HPy getitem(long idx) {
        long idx_pos = idx >= 0 ? idx : idx + (long)st.size();
        if (idx_pos >= (long)st.size() || idx_pos < 0) {
            char msg[256];
            snprintf(msg, 256, "set getitem index out of range (size=%ld, idx=%ld)", st.size(), idx);
            return HPyErr_SetString(
                _g_ctx,
                _g_ctx->h_IndexError,
                msg
            );
        }

        #ifdef __GNUC__
            it = st.find_by_order(idx_pos);
        #else
            it = st.begin();
            for (int i = 0; i < idx_pos; i++) it++;
        #endif

        return as_py(*it);
    }

    HPy pop(long idx) {
        long idx_pos = idx >= 0 ? idx : idx + (long)st.size();
        if (idx_pos >= (long)st.size() || idx_pos < 0) {
            char msg[256];
            snprintf(msg, 256, "set pop index out of range (size=%ld, idx=%ld)", st.size(), idx);
            return HPyErr_SetString(
                _g_ctx,
                _g_ctx->h_IndexError,
                msg
            );
        }

        #ifdef __GNUC__
            it = st.find_by_order(idx_pos);
        #else
            it = st.begin();
            for (int i = 0; i < idx_pos; i++) it++;
        #endif

        HPy res = as_py(*it);
        it = st.erase(it);
        return res;
    }

    long index(HPy x) {
        ll x_c = as_c(x);

        #ifdef __GNUC__
            return st.order_of_key(x_c);
        #else
            long res = 0;
            pb_set::iterator it2 = st.begin();
            while (it2 != st.end() && comp(*it2, x_c)) it2++, res++;
            return res;
        #endif
    }
};

typedef struct {
    Set4PyLong* st;
} SetObject;

HPyType_HELPERS(SetObject)

HPyDef_SLOT(Set_new, Set_new_impl, HPy_tp_new)
HPy Set_new_impl(HPyContext *ctx, HPy cls, HPy *args, HPy_ssize_t nargs, HPy Kw)
{
    if (!HPyArg_Parse(ctx, NULL, args, nargs, ""))
        return HPy_NULL;

    _g_ctx = ctx;
    SetObject* set;

    HPy h_set = HPy_New(ctx, cls, &set);

    _g_ctx = ctx;
    set->st = new Set4PyLong();

    if (HPy_IsNull(h_set))
        return HPy_NULL;
    return h_set;
}

HPyDef_METH(Set_add, "add", Set_add_impl, HPyFunc_O)
HPy Set_add_impl(HPyContext *ctx, HPy self, HPy elt)
{
    SetObject *set = SetObject_AsStruct(ctx, self);

    _g_ctx = ctx;
    bool res = set->st->add(elt);

    return HPyBool_FromLong(ctx, res);
}

HPyDef_METH(Set_remove, "remove", Set_remove_impl, HPyFunc_O)
HPy Set_remove_impl(HPyContext *ctx, HPy self, HPy elt)
{
    SetObject *set = SetObject_AsStruct(ctx, self);

    _g_ctx = ctx;
    HPy res = set->st->remove(elt);

    return res;
}

HPyDef_METH(Set_search_higher_equal, "search_higher_equal", Set_search_higher_equal_impl, HPyFunc_O)
HPy Set_search_higher_equal_impl(HPyContext *ctx, HPy self, HPy elt)
{
    SetObject *set = SetObject_AsStruct(ctx, self);

    _g_ctx = ctx;
    HPy res = set->st->search_higher_equal(elt);

    return res;
}

HPyDef_METH(Set_min, "min", Set_min_impl, HPyFunc_NOARGS)
HPy Set_min_impl(HPyContext *ctx, HPy self)
{
    SetObject *set = SetObject_AsStruct(ctx, self);

    _g_ctx = ctx;
    HPy res = set->st->min();

    return res;
}

HPyDef_METH(Set_max, "max", Set_max_impl, HPyFunc_NOARGS)
HPy Set_max_impl(HPyContext *ctx, HPy self)
{
    SetObject *set = SetObject_AsStruct(ctx, self);

    _g_ctx = ctx;
    HPy res = set->st->max();

    return res;
}

HPyDef_METH(Set_pop_min, "pop_min", Set_pop_min_impl, HPyFunc_NOARGS)
HPy Set_pop_min_impl(HPyContext *ctx, HPy self)
{
    SetObject *set = SetObject_AsStruct(ctx, self);

    _g_ctx = ctx;
    HPy res = set->st->pop_min();

    return res;
}

HPyDef_METH(Set_pop_max, "pop_max", Set_pop_max_impl, HPyFunc_NOARGS)
HPy Set_pop_max_impl(HPyContext *ctx, HPy self)
{
    SetObject *set = SetObject_AsStruct(ctx, self);

    _g_ctx = ctx;
    HPy res = set->st->pop_max();

    return res;
}

HPyDef_SLOT(Set_len, Set_len_impl, HPy_mp_length)
long Set_len_impl(HPyContext *ctx, HPy self)
{
    SetObject *set = SetObject_AsStruct(ctx, self);

    _g_ctx = ctx;
    size_t res = set->st->len();

    return res;
}

HPyDef_METH(Set_next, "next", Set_next_impl, HPyFunc_NOARGS)
HPy Set_next_impl(HPyContext *ctx, HPy self)
{
    SetObject *set = SetObject_AsStruct(ctx, self);

    _g_ctx = ctx;
    HPy res = set->st->iter_next();

    return res;
}

HPyDef_METH(Set_prev, "prev", Set_prev_impl, HPyFunc_NOARGS)
HPy Set_prev_impl(HPyContext *ctx, HPy self)
{
    SetObject *set = SetObject_AsStruct(ctx, self);

    _g_ctx = ctx;
    HPy res = set->st->iter_prev();

    return res;
}

HPyDef_METH(Set_to_list, "to_list", Set_to_list_impl, HPyFunc_NOARGS)
HPy Set_to_list_impl(HPyContext *ctx, HPy self)
{
    SetObject *set = SetObject_AsStruct(ctx, self);

    _g_ctx = ctx;
    HPy res = set->st->to_list();

    return res;
}

HPyDef_METH(Set_get, "get", Set_get_impl, HPyFunc_NOARGS)
HPy Set_get_impl(HPyContext *ctx, HPy self)
{
    SetObject *set = SetObject_AsStruct(ctx, self);

    _g_ctx = ctx;
    HPy res = set->st->get();

    return res;
}

HPyDef_METH(Set_erase, "erase", Set_erase_impl, HPyFunc_NOARGS)
HPy Set_erase_impl(HPyContext *ctx, HPy self)
{
    SetObject *set = SetObject_AsStruct(ctx, self);

    _g_ctx = ctx;
    HPy res = set->st->erase();

    return res;
}

HPyDef_METH(Set_copy, "copy", Set_copy_impl, HPyFunc_NOARGS)
HPy Set_copy_impl(HPyContext *ctx, HPy self)
{
    SetObject *set = SetObject_AsStruct(ctx, self);
    SetObject* set2;

    _g_ctx = ctx;
    HPy h_set = HPy_New(ctx, HPy_Type(ctx, self), &set2);

    _g_ctx = ctx;
    set2->st = new Set4PyLong(*set->st);

    if (HPy_IsNull(h_set))
        return HPy_NULL;
    return h_set;
}

HPyDef_SLOT(Set_getitem, Set_getitem_impl, HPy_sq_item)
HPy Set_getitem_impl(HPyContext *ctx, HPy self, HPy_ssize_t key)
{
    SetObject *set = SetObject_AsStruct(ctx, self);

    _g_ctx = ctx;
    HPy res = set->st->getitem(key);

    return res;
}

HPyDef_METH(Set_pop, "pop", Set_pop_impl, HPyFunc_O)
HPy Set_pop_impl(HPyContext *ctx, HPy self, HPy key)
{
    SetObject *set = SetObject_AsStruct(ctx, self);

    _g_ctx = ctx;
    HPy res = set->st->pop(HPyLong_AsLong(ctx, key));

    return res;
}

HPyDef_METH(Set_index, "index", Set_index_impl, HPyFunc_O)
HPy Set_index_impl(HPyContext *ctx, HPy self, HPy key)
{
    SetObject *set = SetObject_AsStruct(ctx, self);

    _g_ctx = ctx;
    long res = set->st->index(key);

    return HPyLong_FromLong(ctx, res);
}


HPyDef_SLOT(Set_repr, Set_repr_impl, HPy_tp_repr)
static HPy Set_repr_impl(HPyContext *ctx, HPy self)
{
    SetObject *set = SetObject_AsStruct(ctx, self);

    _g_ctx = ctx;
    HPy lst = set->st->to_list();

    HPy lst_repr = HPy_Repr(ctx, lst);

    // in 0.0.4, HPyUnicode_FromFormat is not available
    return HPy_FromPyObject(ctx, PyUnicode_FromFormat("Set(%U)", HPy_AsPyObject(ctx, lst_repr)));
}


static HPyDef *module_defines[] = {
    NULL
};

static HPyModuleDef moduledef = {
    .name = "fastset",
    .doc = "Fast Set for Python",
    .size = -1,
    .defines = module_defines
};

static HPyDef *set_type_defines[] = {
    &Set_new,
    &Set_add,
    &Set_remove,
    &Set_search_higher_equal,
    &Set_min,
    &Set_max,
    &Set_pop_min,
    &Set_pop_max,
    &Set_len,
    &Set_next,
    &Set_prev,
    &Set_to_list,
    &Set_get,
    &Set_erase,
    &Set_copy,
    &Set_getitem,
    &Set_pop,
    &Set_index,
    &Set_repr,
    NULL
};

static HPyType_Spec set_type_spec = {
    .name = "fastset.Set",
    .basicsize = sizeof(SetObject),
    .flags = HPy_TPFLAGS_DEFAULT,
    .defines = set_type_defines
};

HPy_MODINIT(fastset)
static HPy init_fastset_impl(HPyContext *ctx)
{
    HPy m, h_set_type;
    m = HPyModule_Create(ctx, &moduledef);
    if (HPy_IsNull(m))
        return HPy_NULL;
    h_set_type = HPyType_FromSpec(ctx, &set_type_spec, NULL);
    if (HPy_IsNull(h_set_type))
      return HPy_NULL;
    HPy_SetAttr_s(ctx, m, "Set", h_set_type);
    return m;
}
