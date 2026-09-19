# model-lifecycle (opt-in afwfcgi)

Compiled `on*` scripts live on the **location adapter** (`isModelLocation`
file backend), not on the model service. Stop/start of `adapter-model`
alone keeps the old compile. A disk replace of `_AdaptiveModel_` is
visible to `get_object` on the file adapter immediately; the live model
compile does not change until the location adapter is recreated (and
that has only been reliable here when the model service was already
stopped).

Bad `onGetObject` / `defaultValue` / `onGetProperty` does **not** fail
`service_start` (status stays `running`). The parse error is thrown on
the first `get_object`. The mapped file backend still serves.

`onGetObject` that `evaluate(compile<script>(...))` returns a managed
value pinned on the request dest `p` (no `clone_unmanaged` at
`compiled_value` evaluate).
