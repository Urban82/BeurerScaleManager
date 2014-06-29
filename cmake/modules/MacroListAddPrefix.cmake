function(list_add_prefix list_name prefix)
    set(${list_name}_TMP)

    foreach(l ${${list_name}})
        list(APPEND ${list_name}_TMP ${prefix}${l})
    endforeach(l ${${list_name}})

    set(${list_name} "${${list_name}_TMP}" PARENT_SCOPE)
    unset(${list_name}_TMP)
endfunction(list_add_prefix list prefix)