/*
 * ex: set ro:
 * DO NOT EDIT.
 * generated by smc (http://smc.sourceforge.net/)
 * from file : coherence.sm
 */

#ifndef _H_COHERENCE_SM
#define _H_COHERENCE_SM

#include <assert.h>
#define STATEMAP_DEBUG 1
#include <statemap.h>

struct node;
struct line;

struct coherence;
struct coherenceContext;

struct coherenceState
{

    void(*MESI)(struct coherenceContext*);
    void(*MESIF)(struct coherenceContext*);
    void(*MOESI)(struct coherenceContext*);
    void(*MOSI)(struct coherenceContext*);
    void(*MSI)(struct coherenceContext*);
    void(*a_del)(struct coherenceContext*, struct node*, unsigned long, struct line*);
    void(*a_modify)(struct coherenceContext*, struct node*, unsigned long, struct line*);
    void(*a_read)(struct coherenceContext*, struct node*, unsigned long, struct line*);
    void(*i_del)(struct coherenceContext*, struct node*, unsigned long, struct line*);
    void(*i_modify)(struct coherenceContext*, struct node*, unsigned long, struct line*);
    void(*i_read)(struct coherenceContext*, struct node*, unsigned long, struct line*);

    void(*Default)(struct coherenceContext*);
    STATE_MEMBERS
};

extern const struct coherenceState Init_Start;
extern const struct coherenceState MSI_I;
extern const struct coherenceState MSI_M;
extern const struct coherenceState MSI_S;
extern const struct coherenceState MESI_I;
extern const struct coherenceState MESI_M;
extern const struct coherenceState MESI_S;
extern const struct coherenceState MESI_E;
extern const struct coherenceState MOSI_I;
extern const struct coherenceState MOSI_M;
extern const struct coherenceState MOSI_S;
extern const struct coherenceState MOSI_O;
extern const struct coherenceState MOESI_I;
extern const struct coherenceState MOESI_M;
extern const struct coherenceState MOESI_S;
extern const struct coherenceState MOESI_E;
extern const struct coherenceState MOESI_O;
extern const struct coherenceState MESIF_I;
extern const struct coherenceState MESIF_M;
extern const struct coherenceState MESIF_S;
extern const struct coherenceState MESIF_E;
extern const struct coherenceState MESIF_F;

struct coherenceContext
{
    struct coherence *_owner;
    FSM_MEMBERS(coherence)
};

#ifdef NO_COHERENCE_SM_MACRO
extern void coherenceContext_Init(struct coherenceContext*, struct coherence*);
extern void coherenceContext_MESI(struct coherenceContext*);
extern void coherenceContext_MESIF(struct coherenceContext*);
extern void coherenceContext_MOESI(struct coherenceContext*);
extern void coherenceContext_MOSI(struct coherenceContext*);
extern void coherenceContext_MSI(struct coherenceContext*);
extern void coherenceContext_a_del(struct coherenceContext*, struct node*, unsigned long, struct line*);
extern void coherenceContext_a_modify(struct coherenceContext*, struct node*, unsigned long, struct line*);
extern void coherenceContext_a_read(struct coherenceContext*, struct node*, unsigned long, struct line*);
extern void coherenceContext_i_del(struct coherenceContext*, struct node*, unsigned long, struct line*);
extern void coherenceContext_i_modify(struct coherenceContext*, struct node*, unsigned long, struct line*);
extern void coherenceContext_i_read(struct coherenceContext*, struct node*, unsigned long, struct line*);
#else
#define coherenceContext_Init(fsm, owner) \
    FSM_INIT((fsm), &Init_Start); \
    (fsm)->_owner = (owner);

#define coherenceContext_MESI(fsm) \
    assert(getState(fsm) != NULL); \
    setTransition((fsm), "MESI"); \
    getState(fsm)->MESI(fsm); \
    setTransition((fsm), NULL);

#define coherenceContext_MESIF(fsm) \
    assert(getState(fsm) != NULL); \
    setTransition((fsm), "MESIF"); \
    getState(fsm)->MESIF(fsm); \
    setTransition((fsm), NULL);

#define coherenceContext_MOESI(fsm) \
    assert(getState(fsm) != NULL); \
    setTransition((fsm), "MOESI"); \
    getState(fsm)->MOESI(fsm); \
    setTransition((fsm), NULL);

#define coherenceContext_MOSI(fsm) \
    assert(getState(fsm) != NULL); \
    setTransition((fsm), "MOSI"); \
    getState(fsm)->MOSI(fsm); \
    setTransition((fsm), NULL);

#define coherenceContext_MSI(fsm) \
    assert(getState(fsm) != NULL); \
    setTransition((fsm), "MSI"); \
    getState(fsm)->MSI(fsm); \
    setTransition((fsm), NULL);

#define coherenceContext_a_del(fsm, n, i, l) \
    assert(getState(fsm) != NULL); \
    setTransition((fsm), "a_del"); \
    getState(fsm)->a_del(fsm, n, i, l); \
    setTransition((fsm), NULL);

#define coherenceContext_a_modify(fsm, n, i, l) \
    assert(getState(fsm) != NULL); \
    setTransition((fsm), "a_modify"); \
    getState(fsm)->a_modify(fsm, n, i, l); \
    setTransition((fsm), NULL);

#define coherenceContext_a_read(fsm, n, i, l) \
    assert(getState(fsm) != NULL); \
    setTransition((fsm), "a_read"); \
    getState(fsm)->a_read(fsm, n, i, l); \
    setTransition((fsm), NULL);

#define coherenceContext_i_del(fsm, n, i, l) \
    assert(getState(fsm) != NULL); \
    setTransition((fsm), "i_del"); \
    getState(fsm)->i_del(fsm, n, i, l); \
    setTransition((fsm), NULL);

#define coherenceContext_i_modify(fsm, n, i, l) \
    assert(getState(fsm) != NULL); \
    setTransition((fsm), "i_modify"); \
    getState(fsm)->i_modify(fsm, n, i, l); \
    setTransition((fsm), NULL);

#define coherenceContext_i_read(fsm, n, i, l) \
    assert(getState(fsm) != NULL); \
    setTransition((fsm), "i_read"); \
    getState(fsm)->i_read(fsm, n, i, l); \
    setTransition((fsm), NULL);
#endif

#endif

/*
 * Local variables:
 *  buffer-read-only: t
 * End:
 */