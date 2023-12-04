FROM nginx

COPY ezines /usr/share/nginx/html/

COPY nginx.conf /etc/nginx/nginx.conf
EXPOSE 80

CMD ["nginx", "-g", "daemon off;"]
