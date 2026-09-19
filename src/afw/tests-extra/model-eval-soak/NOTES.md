# model-eval-soak

`onGetObject` runs `evaluate(compile<script>(...))` every request.
The compiled hook lives on the location adapter; each request gets a
new managed result pinned on dest `p`. Mutating one get must not
change the next.
