---
title: NETA
description: The Nested English Typing Approach (NETA)
---

## Overview

The Nested English Typing Approach (NETA) originated in [Aten](https://www.projectaten.com/aten) as an attempt to describe the chemical environment of a given atom in a molecule based on its connectivity, and in a clear and readable way. In Dissolve its purpose is to describe how to recognise atoms within molecules described by specific forcefields, and to subsequently allow all forcefield terms to be applied automatically.

## Basics

Each atom type in a forcefield is described by a chemical element, a unique integer index, and a NETA string describing how to recognise it. The NETA string describes the environment of the atom to recognise beginning from the atom itself - for instance, specifying the number of bonds it should have - and then "walks" to neighbouring atoms and describes what their local environment should be, and then their neighbours, and so on. The depth to which neighbours are checked depends on how complicated the molecule / forcefield is, and how unique the atom we are describing happens to be. Parentheses surround NETA elements that describe the atom which immediately precedes it, and in principle can be used to nest descriptions to any depth.

For example, we could uniquely describe the alcohol hydrogen of methanol (CH<sub>3</sub>OH) with the NETA string `-O` as this is enough to differentiate it from the aliphatic hydrogens on the carbon. If we wanted to, we could nest descriptions to cover the whole molecule, giving `-O(-C(nh=3))`. Starting off from our target alcohol hydrogen, this can be read as "is bound to an oxygen, which itself is bound to a carbon, which itself has three attached hydrogen atoms".

## Type References

Uniquely describing atoms based on connectivity can get quite laborious when trying to tackle larger molecules, since this often requires the repetition of NETA fragments to identify atoms that have already been described. In most situations, using _type references_ can dramatically reduce the amount of NETA required, as previously-defined atoms can be referenced by their unique integer index when describing other atoms. A good tactic to use is to describe the most unique, "central" atom in a molecule as completely as possible, and then reference it when describing other atoms.

Type references are written as `&nn` where `nn` is the unique index of the type.

## Scoring

When it comes to actually trying to recognise an atom from its type description, all of the types in the forcefield that have the same element are scored individually, with the highest score being the one that is finally selected as the best match. Each separate part of a NETA description scores one or more points, so more complete / complex descriptions result in higher scores.

## NETA Reference

### "Bound To" (`-`)

The `-` keyword specifies a connection to another atom that the current atom must have. The other atom is expressed as a single element or type reference, or a mixed list of both enclosed in square brackets.


#### Examples

```r
-C            # ... is bound to a carbon atom
-[P,S]        # ... is bound to a phosphorous or sulphur atom
-[&6,&7,N]    # ... is bound to atom type with id 6 or 7, or a nitrogen
```

### `ring`

On its own, the `ring` keyword indicates that the atom should be present in some sort of cyclic moiety of any size within the molecule. The exact size of the ring can be specified using the `size` modifier within parentheses immediately following the keyword. Within the same parentheses, the exact composition of the ring can be provided with a comma-separated list of elements or type references.

For example:

```r
ring(size=4)      # ... exists within a four-atom ring
ring(size>=6)     # ... exists within a ring of six atoms or greater
ring(C,C,N,C,C)   # ... exists in a ring containing the elemental sequence `C`, `C`, `N`, `C`, `C`
```

## Further Examples

TODO
