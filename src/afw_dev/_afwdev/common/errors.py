# -*- coding: utf-8 -*-
##
# @file errors.py
# @ingroup afwdev_common
# @brief Structured exceptions for afwdev (issue #61).
# @details Distinguishes Adaptive/AFW error objects from pure Python failures
#          so the test runner can extract a parseable message/object without
#          treating every failure as a bare string or generic Exception.
#

"""
Hierarchy (all subclass Exception so existing ``except Exception`` still works):

* AfwdevError — base; optional ``object`` (Adaptive error dict) and ``message``
* AfwAdaptiveError — error produced by Adaptive / afw JSON / test_script
* AfwdevProcessError — subprocess / host abnormal exit
* AfwdevRunnerError — harness infrastructure (bail, missing script, load, …)
"""


class AfwdevError(Exception):
    """Base structured error for afwdev tooling."""

    def __init__(self, message, object=None, cause=None):
        # ``object`` is the Adaptive error object when present (issue #61).
        msg = message if message is not None else ""
        if not isinstance(msg, str):
            msg = str(msg)
        super(AfwdevError, self).__init__(msg)
        self.message = msg
        self.object = object
        self.cause = cause

    def to_error_dict(self):
        """
        Adaptive-shaped error dict for test_script-style responses / printers.
        """
        if isinstance(self.object, dict):
            d = dict(self.object)
            if not d.get("message"):
                d["message"] = self.message
            if not d.get("id"):
                d["id"] = type(self).__name__
            return d
        return {
            "id": type(self).__name__,
            "message": self.message,
        }

    def __str__(self):
        return self.message


class AfwAdaptiveError(AfwdevError):
    """
    Failure originating from Adaptive Framework (perform/eval JSON error,
    test_script case, assertion, etc.).
    """

    def __init__(self, message, object=None, cause=None):
        super(AfwAdaptiveError, self).__init__(
            message, object=object, cause=cause)


class AfwdevProcessError(AfwdevError):
    """Subprocess or host process failed (nonzero exit, signal, spawn)."""

    def __init__(self, message, object=None, cause=None, returncode=None):
        super(AfwdevProcessError, self).__init__(
            message, object=object, cause=cause)
        self.returncode = returncode


class AfwdevRunnerError(AfwdevError):
    """Test runner / harness error (bail, bad marker, missing assets, …)."""

    def __init__(self, message, object=None, cause=None):
        super(AfwdevRunnerError, self).__init__(
            message, object=object, cause=cause)


def error_message(err):
    """Best one-line message from Exception / AfwdevError / string / None."""
    if err is None:
        return None
    if isinstance(err, AfwdevError):
        return err.message
    if isinstance(err, BaseException):
        return str(err) or type(err).__name__
    if isinstance(err, dict):
        return err.get("message") or err.get("id") or str(err)
    return str(err)


def error_to_dict(err):
    """
    Adaptive-shaped error dict for printers, or None.
    """
    if err is None:
        return None
    if isinstance(err, AfwdevError):
        return err.to_error_dict()
    if isinstance(err, dict):
        d = dict(err)
        if "message" not in d:
            d["message"] = d.get("id") or "error"
        return d
    if isinstance(err, BaseException):
        return {
            "id": type(err).__name__,
            "message": str(err) or type(err).__name__,
        }
    if isinstance(err, str):
        return {"id": "afwdev", "message": err}
    return {"id": "afwdev", "message": str(err)}


def adaptive_error_from_response(response, default_message=None):
    """
    Build AfwAdaptiveError from a perform/eval JSON body if status is error.
    Returns None if response is not an Adaptive error payload.
    """
    if not isinstance(response, dict):
        return None
    if response.get("status") != "error":
        # Nested action error
        actions = response.get("actions")
        if isinstance(actions, list):
            for act in actions:
                if isinstance(act, dict) and act.get("status") == "error":
                    obj = act.get("error") if isinstance(
                        act.get("error"), dict) else act
                    msg = None
                    if isinstance(obj, dict):
                        msg = obj.get("message")
                    return AfwAdaptiveError(
                        msg or default_message or "Adaptive action error",
                        object=obj if isinstance(obj, dict) else None,
                    )
        return None
    obj = response.get("error")
    if isinstance(obj, dict):
        return AfwAdaptiveError(
            obj.get("message") or default_message or "Adaptive error",
            object=obj,
        )
    return AfwAdaptiveError(
        default_message or str(obj) or "Adaptive error",
        object={"message": str(obj)} if obj is not None else None,
    )


def wrap_exception(exc, default_cls=AfwdevRunnerError):
    """Re-wrap bare Exception as default_cls; pass through AfwdevError."""
    if isinstance(exc, AfwdevError):
        return exc
    if isinstance(exc, BaseException):
        return default_cls(str(exc) or type(exc).__name__, cause=exc)
    return default_cls(str(exc))
