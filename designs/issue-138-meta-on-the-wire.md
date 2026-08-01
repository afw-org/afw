# Issue #138 — Meta on the wire (pad)

**GitHub:** [#138](https://github.com/afw-org/afw/issues/138)  
**Labels:** `bug`, `enhancement`  
**Assignees:** mike000000000, JeremyGrieshop  
**Status:** Opened for design + multi-PR work; **does not block #38**.

## One-line problem

Sideband **meta** is real and load-bearing (adapters, object options, reconcile, UI/type info), but map content types spell it as reserved property name `"_meta_"`, which collides with free property names. Fix is encoding/view packaging—not banning the name in script or `afw_object`.

## Remember when touching this

- **Bug + enhancements:** collision fix and better delivery of rich type info / options / clients.  
- **#38** only adds another way to hit an **existing** problem.  
- **Do not** ban `"_meta_"` in language or normal property set.  
- **Interim:** app data that round-trips map CTs should avoid property name `"_meta_"`.  
- **Rich typing / `objectTypes` etc.:** historical goal was type-aware / dynamic client UI; XML was early primary CT; JSON won; large options bloat instances—discuss instance meta vs type fetch/cache without mandating one UI architecture.  
- **reconcile_object**, file/LMDB essential meta, admin + TS client all in the blast radius.  
- Full discussion lives on the **GitHub issue body**; expand this pad when implementation design starts.

## Relationship to #38

Implement expression property names without waiting on #138. Literal `_meta_:` peel stays; computed `"_meta_"` = normal property.
