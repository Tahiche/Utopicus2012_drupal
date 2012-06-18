<?php // user login de toboggan, bloque copiado para que funcione en magento, etc. Mirar tema rutas e idioma ?>
  <div id="logindiv" >
    <form id="user-login-form" method="post" accept-charset="UTF-8" action="/es/node?destination=node">
<div><div class="toboggan-container" id="toboggan-container">

<div class="user-login-block " id="toboggan-login" style="display: none;"><div id="edit-name-wrapper" class="form-item">
 <label for="edit-name">Usuario o e-mail: <span title="Este campo es obligatorio." class="form-required">*</span></label>
 <input type="text" class="form-text required" value="" size="15" id="edit-name" name="name" maxlength="60">
</div>
<div id="edit-pass-wrapper" class="form-item">
 <label for="edit-pass">Contraseña: <span title="Este campo es obligatorio." class="form-required">*</span></label>
 <span class="input-placeholder-text" style="color: rgb(94, 94, 94); position: absolute;"></span><input type="password" class="form-text required" size="15" maxlength="60" id="edit-pass" name="pass">
</div>
<input type="submit" class="form-submit" value="Iniciar sesión" id="edit-submit" name="op">
<div class="item-list"><ul><li class="first"><a title="Crear una nueva cuenta de usuario." href="/es/user/register">Nueva cuenta</a></li>
<li class="last"><a title="Solicita una contraseña nueva por correo electrónico." href="/es/user/password">Solicitar nueva contraseña</a></li>
</ul></div><input type="hidden" value="form-f8af5d7258e8d701fb9ff79438a184f5" id="form-f8af5d7258e8d701fb9ff79438a184f5" name="form_build_id">
<input type="hidden" value="user_login_block" id="edit-user-login-block" name="form_id">
</div></div>
</div></form>




<div class="user-login-block " id="toboggan-user" style="display: none;">

<?php 
// si estoy en DRupal tyengo id, desde Moodle muestro un sólo link
global $user;
if($user->uid): 
echo "<label><b>USUARIO</b> </label>";
echo l(">> Ver mi perfil", "user/".$user->uid);
echo l(">> Editar mi perfil", "user/".$user->uid."/profile/perfil_usuario");
?>
<?php else:?>
<a href="/user">>> Cuenta</a>
<?php endif; ?>
<a href="/logout">>> Cerrar Sesión</a>
</div>
  </div>