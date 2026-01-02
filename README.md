## TODO

- [ ] Deal with noise (floating point issues)
- [ ] Reflection might be buggy (ring artifact around sphere)

## Before Moving On

- [ ] Verify shadows work properly without noise (under)
- [ ] What is going on with all the noise in patterns?
  - wait the noise disappeared in some renders???
- [ ] Transparency doesnt seem to be working as intended (is it shadowing?)

## Known Bugs

- Shadow acne on extremely distorted objects (i.e. sphere scaled to act as plane) caused by floating-point error. Shadow acne was generally fixed for most objects via scale-aware epsilon when calculating `over_point`. Error with normals in distorted objects prevents this fix from working, so this will have to be addressed in physically-based path tracer.
- Salt-and-pepper noise with checkerboard pattern caused by floating-point error in conjunction with large world-space point values and the use of floor causing the wrong color to be selected.
  - Recently fixed by passing `comps.over_point` to `phong_lighting` in `shade_hit` instead of the raw intersection point.
